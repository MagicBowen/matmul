/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef ITERATE_CONTROLLER_H
#define ITERATE_CONTROLLER_H

#include "matmul_keyword.h"

namespace matmul {

template <typename IMPL, const auto& MM_CFG>
class IterateController {
public:
    bool MoveNext() {
        if (IsFinished()) return false;

        if (++curN_ >= MATMUL_CONST_PARAM_VAR.nIter_) {
            curN_ = 0;
            if (++curM_ >= MATMUL_CONST_PARAM_VAR.mIter_) {
                return false;
            }
        }
        return true;
    }

    uint32_t GetRowIndex() const {
        return curM_;
    }

    uint32_t GetColIndex() const {
        return curN_;
    }

private:
    bool IsFinished() const {
        return curM_ >= MATMUL_CONST_PARAM_VAR.mIter_;
    }

private:
    uint32_t curM_{0};
    uint32_t curN_{0};
    
    uint32_t curStepM_;
    uint32_t curStepN_;
    
    uint32_t stepMIdx_;
    uint32_t stepNIdx_;
};

}

#endif
