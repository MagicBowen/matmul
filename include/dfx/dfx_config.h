/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_CONFIG_H
#define MATMUL_DFX_CONFIG_H

#include "dfx/handlers/dfx_trace_handler.h"
#include "dfx/handlers/dfx_statistic_handler.h"
#include "dfx/handlers/dfx_chain_handler.h"

namespace matmul {

struct DfxConfig {
    
    static constexpr bool ENABLE = true;

    using EnabledHandlers = DfxChainHandler <
        DfxTraceHandler, 
        DfxStatisticHandler 
    >;
};

}

#endif
