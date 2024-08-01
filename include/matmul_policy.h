/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_POLICY_H
#define MATMUL_POLICY_H

#include "modules/scheduler.h"
#include "modules/iterate_controller.h"
#include "modules/copy_cube_in.h"
#include "modules/copy_in_buffer.h"
#include "modules/split_load.h"
#include "modules/mmad.h"
#include "modules/co1_buffer.h"
#include "modules/copy_cube_out.h"

namespace matmul {

struct MatmulContext {
};

template<typename IMPL, typename A_TYPE, typename B_TYPE, typename C_TYPE, typename BIAS_TYPE, const auto& MM_CFG, typename MM_CB>
struct MatmulPolicyDefault {

    ////////////////////////////////////////////////////////////////////
    using INPUT_TYPE_A      =   matmul::MatmulInputTypeA<A_TYPE>;
    using INPUT_TYPE_B      =   matmul::MatmulInputTypeB<B_TYPE>;
    using MMAD_SRC_TYPE     =   typename SrcType<INPUT_TYPE_A, INPUT_TYPE_B>::Type;
    
    ////////////////////////////////////////////////////////////////////
    using IterateController =   matmul::IterateController<IMPL, MM_CFG>;

    using CopyCubeInA       =   matmul::CopyCubeIn<IMPL, INPUT_TYPE_A, MM_CFG>;
    using CopyCubeInB       =   matmul::CopyCubeIn<IMPL, INPUT_TYPE_B, MM_CFG>;

    using CopyInBufferA     =   matmul::CopyInBuffer<IMPL, INPUT_TYPE_A, MM_CFG>;
    using CopyInBufferB     =   matmul::CopyInBuffer<IMPL, INPUT_TYPE_B, MM_CFG>;

    using SplitLoadA        =   matmul::SplitLoad<INPUT_TYPE_A, MM_CFG>;
    using SplitLoadB        =   matmul::SplitLoad<INPUT_TYPE_B, MM_CFG>;

    using MMad              =   matmul::MMad<IMPL, MMAD_SRC_TYPE, typename C_TYPE::T, MM_CFG>;
    using Co1Buffer         =   matmul::Co1Buffer<IMPL, C_TYPE, MM_CFG>;
    using CopyCubeOut       =   matmul::CopyCubeOut<IMPL, C_TYPE, MM_CFG>;

    using Scheduler         =   matmul::Scheduler<IMPL, INPUT_TYPE_A, INPUT_TYPE_B, C_TYPE, MM_CFG>;
    using Context           =   matmul::MatmulContext;
};

}

#endif
