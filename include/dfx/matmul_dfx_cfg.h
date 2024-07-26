/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_CFG_H
#define MATMUL_DFX_CFG_H

#include "dfx/handlers/matmul_dfx_print_handler.h"
#include "dfx/handlers/matmul_dfx_statistic_handler.h"
#include "dfx/handlers/matmul_dfx_chain_handler.h"

namespace matmul {

struct MatmulDfxCfg {
    
    static constexpr bool ENABLE = true;

    using EnabledHandlers = MatmulDfxChainHandler< 
        MatmulDfxPrintHandler, 
        MatmulDfxStatisticHandler 
    >;
};

}

#endif
