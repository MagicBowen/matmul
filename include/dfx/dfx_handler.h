/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_HANDLER_H
#define MATMUL_DFX_HANDLER_H

#include "dfx/dfx_config.h"

namespace matmul {

struct DfxHandler {
    template<typename... Args>
    static void PreCall(const DfxFuncInfo& info, Args&&... args) {
        DfxConfig::EnabledHandlers::PreCall(info, std::forward<Args>(args)...);
    }

    template<typename RT>
    static void PostCall(const DfxFuncInfo& info, const RT& ret) {
        DfxConfig::EnabledHandlers::PostCall(info, ret);
    }

    static void PostCall(const DfxFuncInfo& info) {
        DfxConfig::EnabledHandlers::PostCall(info);
    }
};

}

#endif
