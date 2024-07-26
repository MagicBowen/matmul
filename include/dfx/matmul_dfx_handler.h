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
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "【DFX FUNCTION ENTER】: function : " << module << "::" << func << ", params : ";
        ((std::cout << std::forward<Args>(args) << "; "), ...);
        std::cout << std::endl;
    }

    template<typename RT>
    static void PostCall(const char* module, const char* func, const RT& ret) {
        std::cout  << "【DFX FUNCTION EXIT 】: function : " << module << "::" << func << ", return : " << ret << ";" << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
    }

    static void PostCall(const char* module, const char* func) {
        std::cout  << "【DFX FUNCTION EXIT 】: function : " << module << "::" << func << ", return : void" << ";" <<  std::endl;
        std::cout << "------------------------------------------------" << std::endl;
    }
};

}

#endif
