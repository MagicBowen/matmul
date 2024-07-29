/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_PARAMS_H
#define MATMUL_PARAMS_H

#include "matmul_tiling.h"

namespace matmul {

struct MatmulParams {
    const TCubeTiling* tiling;

    uint32_t singleCoreM_{0};
    uint32_t singleCoreN_{0};
    uint32_t singleCoreK_{0};

    uint32_t mIter_{0};
    uint32_t nIter_{0};
    uint32_t kIter_{0};

    uint32_t tailM_;
    uint32_t tailK_;
    uint32_t tailN_;

    uint32_t depthA1_;
    uint32_t depthB1_;
};

}

#endif
