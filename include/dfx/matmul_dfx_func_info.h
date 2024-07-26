/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_FUNC_INFO_H
#define MATMUL_DFX_FUNC_INFO_H

#include <stdint.h>

namespace matmul {

struct MatmulDfxFuncInfo {
    const char* module;
    const char* func;
    uint32_t funcId;
};

}

#endif
