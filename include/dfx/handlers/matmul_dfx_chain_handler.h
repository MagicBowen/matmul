/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_CHAIN_HANDLER_H
#define MATMUL_DFX_CHAIN_HANDLER_H

namespace matmul {

struct MatmulDfxFuncInfo;

template<typename ...HANDLERS> 
struct MatmulDfxChainHandler {
    template<typename... Args>
    static void PreCall(const MatmulDfxFuncInfo& info, Args&&... args) {
        (HANDLERS::PreCall(info, std::forward<Args>(args)...), ...);
    }

    template<typename RT>
    static void PostCall(const MatmulDfxFuncInfo& info, const RT& ret) {
        (HANDLERS::PostCall(info, ret), ...);
    }

    static void PostCall(const MatmulDfxFuncInfo& info) {
        (HANDLERS::PostCall(info), ...);
    }
};

}

#endif
