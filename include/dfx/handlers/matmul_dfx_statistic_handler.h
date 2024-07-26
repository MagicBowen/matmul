/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_STATISTIC_HANDLE_H
#define MATMUL_DFX_STATISTIC_HANDLE_H

#include <iostream>
#include <unordered_map>
#include "dfx/matmul_dfx_func_info.h"

namespace matmul {

struct MatmulDfxStatisticHandler {
    template<typename... Args>
    static void PreCall(const MatmulDfxFuncInfo& info, Args&&... args) {
        funcCallCounts_[GetFullFuncName(info.module, info.func)]++;
    }

    template<typename RT>
    static void PostCall(const MatmulDfxFuncInfo& info, const RT& ret) {
    }

    static void PostCall(const MatmulDfxFuncInfo& info) {
    }

    static void PrintStatistic() {
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "【DFX FUNCTION STATISTIC】: " << std::endl;
        for (const auto& [func, count] : funcCallCounts_) {
            std::cout << "【" << func << "】: " << count << std::endl;
        }
        std::cout << "------------------------------------------------" << std::endl;
    }

    static void Clear() {
        funcCallCounts_.clear();
    }

private:
    static const std::string GetFullFuncName(const std::string& module, const std::string& func) {
        return module + std::string("::") + func;
    }

private:
    static std::unordered_map<std::string, uint32_t> funcCallCounts_;
};

}

#endif
