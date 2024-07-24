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
    void Init() {
    }

    bool MoveNext() {
        if (IsFinished()) return false;
        return true;
    }

    bool IsFinished() const {
        printf("iterate pos {%d : %d} \n", curM, MATMUL_CONST_PARAM_VAR.singleCoreM_);
        return curM >= MATMUL_CONST_PARAM_VAR.singleCoreM_;
    }

private:
    uint32_t curM{0};
};

}

#endif
