/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_HANDLER_H
#define MATMUL_DFX_HANDLER_H

#include <iostream>

namespace matmul {

struct ModuleDFXHandler {
    template<typename... Args>
    static void PreCall(const char* module, const char* func, Args&&... args) {
        std::cout << module << "::" << func << " begin , Params : ";
        ((std::cout << std::forward<Args>(args) << "; "), ...);
        std::cout << std::endl;
    }

    static void PostCall() {
        std::cout << "-----------------------------" << std::endl;
    }
};

}

#endif
