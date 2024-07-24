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
};

}

#endif
