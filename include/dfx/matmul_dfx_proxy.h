/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_PROXY_H
#define MATMUL_DFX_PROXY_H

#include <type_traits>

namespace matmul {

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct MatmulDfxProxy : T {
};

///////////////////////////////////////////////////////////////////////////////
#define MATMUL_DFX_PROXY_FUN_DECL(MODULE, FUNC, M_)                     \
template <typename... Args>                                             \
auto FUNC(Args&&... args) -> std::enable_if_t<!std::is_void_v<          \
decltype((MODULE().MODULE::FUNC)(std::forward<Args>(args)...))>,        \
decltype((MODULE().MODULE::FUNC)(std::forward<Args>(args)...))>{        \
    ModuleDFXHandler::PreCall(#MODULE, #FUNC, std::forward<Args>(args)...);\
    auto ret = (M_.MODULE::FUNC)(std::forward<Args>(args)...);          \
    ModuleDFXHandler::PostCall();                                       \
    return ret;                                                         \
}                                                                       \
template <typename... Args>                                             \
auto FUNC(Args&&... args) -> std::enable_if_t<std::is_void_v<           \
decltype((MODULE().MODULE::FUNC)(std::forward<Args>(args)...))>> {      \
    ModuleDFXHandler::PreCall(#MODULE, #FUNC, std::forward<Args>(args)...);\
    (M_.MODULE::FUNC)(std::forward<Args>(args)...);                     \
    ModuleDFXHandler::PostCall();                                       \
}   

///////////////////////////////////////////////////////////////////////////////
#define MATMUL_DFX_PROXY_REGISTER(MODULE, FUNC)                         \
template <> struct MatmulDfxProxy<MODULE> {                             \
    MatmulDfxProxy(MODULE& module) : proxy{module} {}                   \
    struct FuncProxy {                                                  \
        FuncProxy(MODULE& module) : m_{module} {}                       \
        MATMUL_DFX_PROXY_FUN_DECL(MODULE, FUNC, m_)                     \
    private:                                                            \
        MODULE& m_;                                                     \
    };                                                                  \
    auto operator->() { return &proxy; }                                \
private:                                                                \
    FuncProxy proxy;                                                    \
};                                                                      \
template <> struct MatmulDfxProxy<const MODULE> {                       \
    MatmulDfxProxy(const MODULE& module) : proxy{module} {}             \
    struct FuncProxy {                                                  \
        FuncProxy(const MODULE& module) : m_{module} {}                 \
        MATMUL_DFX_PROXY_FUN_DECL(MODULE, FUNC, m_)                     \
    private:                                                            \
        const MODULE& m_;                                               \
    };                                                                  \
    auto operator->() { return &proxy; }                                \
private:                                                                \
    FuncProxy proxy;                                                    \
}

///////////////////////////////////////////////////////////////////////////////

}

#endif
