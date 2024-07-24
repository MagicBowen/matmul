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

        if (++curN >= MATMUL_CONST_PARAM_VAR.nIter) {
            curN = 0;
            if (++curM >= MATMUL_CONST_PARAM_VAR.mIter) {
                return false;
            }
        }
        return true;
    }

    uint32_t GetRowIndex() const {
        return curM;
    }

    uint32_t GetColIndex() const {
        return curN;
    }

private:
    bool IsFinished() const {
        return curM >= MATMUL_CONST_PARAM_VAR.mIter;
    }

private:
    uint32_t curM{0};
    uint32_t curN{0};
};

}

#endif
