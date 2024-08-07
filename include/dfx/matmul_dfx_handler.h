/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_HANDLER_H
#define MATMUL_DFX_HANDLER_H

#include "dfx/matmul_dfx_cfg.h"

namespace matmul {

struct MatmulDfxHandler {
    template<typename... Args>
    static void PreCall(const MatmulDfxFuncInfo& info, Args&&... args) {
        MatmulDfxCfg::EnabledHandlers::PreCall(info, std::forward<Args>(args)...);
    }

    template<typename RT>
    static void PostCall(const MatmulDfxFuncInfo& info, const RT& ret) {
        MatmulDfxCfg::EnabledHandlers::PostCall(info, ret);
    }

    static void PostCall(const MatmulDfxFuncInfo& info) {
        MatmulDfxCfg::EnabledHandlers::PostCall(info);
    }
};

}

#endif
