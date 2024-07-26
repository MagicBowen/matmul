/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_PRINT_HANDLE_H
#define MATMUL_DFX_PRINT_HANDLE_H

#include <iostream>
#include "dfx/matmul_dfx_func_info.h"

namespace matmul {

inline std::ostream& operator<<(std::ostream& os, const MatmulDfxFuncInfo& info) {
    os << "function(" << info.module << "::" << info.func << ", " << info.funcId << ")";
    return os;
}

struct MatmulDfxPrintHandler {
    template<typename... Args>
    static void PreCall(const MatmulDfxFuncInfo& info, Args&&... args) {
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "【DFX FUNCTION ENTER】: " << info << ", params(";
        if constexpr (sizeof...(args) > 0) {
            ((std::cout << std::forward<Args>(args) << ";"), ...);
        } else {
            std::cout << "void";
        }
        std::cout << ")" << std::endl;
    }

    template<typename RT>
    static void PostCall(const MatmulDfxFuncInfo& info, const RT& ret) {
        std::cout  << "【DFX FUNCTION EXIT 】: " << info << ", return(" << ret << ")" << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
    }

    static void PostCall(const MatmulDfxFuncInfo& info) {
        std::cout  << "【DFX FUNCTION EXIT 】: " << info << ", return(void)" <<  std::endl;
        std::cout << "------------------------------------------------" << std::endl;
    }
};

}

#endif
