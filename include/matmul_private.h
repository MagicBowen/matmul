/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_PRIVATE_H
#define MATMUL_PRIVATE_H

#include "modules/hal_instruction.h"

namespace matmul {

template<typename IMPL, typename A_TYPE, typename B_TYPE, typename C_TYPE, typename BIAS_TYPE, const auto& MM_CFG>
struct MatmulPrivateModules {
    using HalInstruction =   matmul::HalInstruction<IMPL, MM_CFG>;
};

}

#endif
