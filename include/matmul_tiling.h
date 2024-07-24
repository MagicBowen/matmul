/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_TILING_H
#define MATMUL_TILING_H

#include <stdint.h>

namespace matmul {

struct TCubeTiling {
    uint32_t singleCoreM;
    uint32_t singleCoreN;
    uint32_t singleCoreK;
};

}

#endif
