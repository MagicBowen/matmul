/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef ITERATE_CONTROLLER_H
#define ITERATE_CONTROLLER_H

#include "matmul_keyword.h"
#include "matmul_config.h"

namespace matmul {

template <typename IMPL, const auto& MM_CFG>
class IterateController {
    enum {
        ORDER_M = 0,
        ORDER_N = 1,
        ORDER_MAX = 2
    };

    struct State {
        void Reset(uint32_t stepNum, uint32_t iterNum) {
            curIdx_ = 0;
            stepIdx_ = 0;
            curStep_ = stepNum;
            iterNum_ = iterNum;
        }

        uint32_t curIdx_{0};
        uint32_t stepIdx_{0};
        uint32_t curStep_{0};
        uint32_t iterNum_{0};
    } state_[ORDER_MAX];

    static constexpr uint32_t MAIN_ORDER = 
        (MM_CFG.iterateOrder == IterateOrder::ORDER_M) ? ORDER_M : ORDER_N;
        
    static constexpr uint32_t SUB_ORDER = 
        (MM_CFG.iterateOrder == IterateOrder::ORDER_M) ? ORDER_N : ORDER_M;

public:
    void Init() {
        Reset();
    }

    bool ForwardMN() {
        if (IsFirstIterate()) return true;
        return MoveNext();
    }

    template <typename COMPUTE>
    void ReduceK(COMPUTE compute) {
        for (uint32_t k = 0; k < MATMUL_CONTEXT().kIter_; ++k) {
            compute(GetRowIndex(), GetColIndex(), k);
        }
    }

    uint32_t GetRowIndex() const {
        return state_[ORDER_M].curIdx_;
    }

    uint32_t GetColIndex() const {
        return state_[ORDER_N].curIdx_;
    }

    void Reset() {
        state_[ORDER_M].Reset(MM_CFG.stepM, MATMUL_CONTEXT().mIter_);
        state_[ORDER_N].Reset(MM_CFG.stepN, MATMUL_CONTEXT().nIter_);
    }

private:
    bool IsFirstIterate() const {
        return state_[ORDER_M].curIdx_ == 0 && state_[ORDER_N].curIdx_ == 0;
    }

    bool MoveNext() {
       if (++state_[SUB_ORDER].curIdx_ >= state_[SUB_ORDER].stepIdx_ + state_[SUB_ORDER].curStep_) {
            state_[SUB_ORDER].curIdx_ = state_[SUB_ORDER].stepIdx_;
            if (++state_[MAIN_ORDER].curIdx_ >= state_[MAIN_ORDER].iterNum_) {
                state_[MAIN_ORDER].curIdx_ = 0;
                state_[SUB_ORDER].stepIdx_ += state_[SUB_ORDER].curStep_;
                if (state_[SUB_ORDER].stepIdx_ >= state_[SUB_ORDER].iterNum_) {
                    return false;
                }
                state_[SUB_ORDER].curIdx_ = state_[SUB_ORDER].stepIdx_;
            }
        }
        return true;        
    }
};

}

#endif
