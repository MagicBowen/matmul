/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_POLICY_H
#define MATMUL_POLICY_H

#include "modules/iterate_controller.h"
#include "modules/copy_cube_in.h"
#include "modules/copy_in_buffer.h"
#include "modules/scheduler.h"
#include "modules/mmad.h"

namespace matmul {

struct MatmulContext {
};

template<typename IMPL, typename A_TYPE, typename B_TYPE, typename C_TYPE, typename BIAS_TYPE, const auto& MM_CFG, typename MM_CB>
struct MatmulPolicyDefault {
    
    using IterateController =   matmul::IterateController<IMPL, MM_CFG>;

    using CopyCubeInA       =   matmul::CopyCubeIn<IMPL, A_TYPE, MM_CFG, InputTag::LEFT>;
    using CopyCubeInB       =   matmul::CopyCubeIn<IMPL, B_TYPE, MM_CFG, InputTag::RIGHT>;

    template<typename INPUT_TYPE, InputTag TAG>
    using CopyInBuffer      = matmul::CopyInBuffer<IMPL, INPUT_TYPE, MM_CFG, TAG>;

    using MMad              =   matmul::MMad<IMPL, typename A_TYPE::T, typename C_TYPE::T, MM_CFG>;
    using Scheduler         =   matmul::Scheduler<IMPL, MM_CFG>;
    using Context           =   matmul::MatmulContext;
};

}

#endif
