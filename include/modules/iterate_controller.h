/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef ITERATE_CONTROLLER_H
#define ITERATE_CONTROLLER_H

#include "matmul_keyword.h"
#include "matmul_config.h"

namespace matmul {

//////////////////////////////////////////////////////////////////////
template <typename IMPL, const auto& MM_CFG, typename = void>
class IterateController {
public:
    bool MoveNext() {
        if (IsFirstIterate()) return true;

        if (++curN_ >= stepNIdx_ + curStepN_) {
            curN_ = stepNIdx_;
            if (++curM_ >= MATMUL_CONTEXT().mIter_) {
                curM_ = 0;
                stepNIdx_ += curStepN_;
                if (stepNIdx_ >= MATMUL_CONTEXT().nIter_) {
                    return false;
                }
                curN_ = stepNIdx_;
            }
        }
        return true;
    }

    void Reset() {
        curM_ = 0;
        curN_ = 0;
        stepMIdx_ = 0;
        stepNIdx_ = 0;
        curStepM_ = MM_CFG.stepM;
        curStepN_ = MM_CFG.stepN;
    }

    uint32_t GetRowIndex() const {
        return curM_;
    }

    uint32_t GetColIndex() const {
        return curN_;
    }

private:
    bool IsFirstIterate() const {
        return curM_ == 0 && curN_ == 0;
    }

private:
    uint32_t curM_{0};
    uint32_t curN_{0};
    uint32_t stepMIdx_{0};
    uint32_t stepNIdx_{0};
    uint32_t curStepM_{0};
    uint32_t curStepN_{0};
};

//////////////////////////////////////////////////////////////////////
template <typename IMPL, const auto& MM_CFG>
class IterateController<IMPL, MM_CFG, std::enable_if_t<MM_CFG.iterOrder == IterateOrder::ORDER_N>> {
public:
    bool MoveNext() {
        if (IsFirstIterate()) return true;

        if (++curM_ >= stepMIdx_ + curStepM_) {
            curM_ = stepMIdx_;
            if (++curN_ >= MATMUL_CONTEXT().nIter_) {
                curN_ = 0;
                stepMIdx_ += curStepM_;
                if (stepMIdx_ >= MATMUL_CONTEXT().mIter_) {
                    return false;
                }
                curM_ = stepMIdx_;
            }
        }
        return true;
    }

    void Reset() {
        curM_ = 0;
        curN_ = 0;
        stepMIdx_ = 0;
        stepNIdx_ = 0;
        curStepM_ = MM_CFG.stepM;
        curStepN_ = MM_CFG.stepN;
    }

    uint32_t GetRowIndex() const {
        return curM_;
    }

    uint32_t GetColIndex() const {
        return curN_;
    }

private:
    bool IsFirstIterate() const {
        return curM_ == 0 && curN_ == 0;
    }

private:
    uint32_t curM_{0};
    uint32_t curN_{0};
    uint32_t stepMIdx_{0};
    uint32_t stepNIdx_{0};
    uint32_t curStepM_{0};
    uint32_t curStepN_{0};
};

}

#endif
