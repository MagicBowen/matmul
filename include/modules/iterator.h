/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef ITERATOR_H
#define ITERATOR_H

#include "matmul_keyword.h"
#include "matmul_config.h"

namespace matmul {

template <typename IMPL, const auto& MM_CFG>
class Iterator {

    enum {
        ORDER_M = 0,
        ORDER_N = 1,
        ORDER_MAX
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
    bool IsFinished() const {
        return (state_[SUB_ORDER].stepIdx_ >= state_[SUB_ORDER].iterNum_)
        && (state_[MAIN_ORDER].curIdx_ >= state_[MAIN_ORDER].iterNum_);
    }
};

} // namespace matmul

#endif
