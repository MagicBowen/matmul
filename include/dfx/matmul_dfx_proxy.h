/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_PROXY_H
#define MATMUL_DFX_PROXY_H

#include <type_traits>
#include "dfx/matmul_dfx_handler.h"

namespace matmul {

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename M>
struct MatmulDfxProxy : M {
    auto operator->() { return this; }
    operator T*() { return this; }
};

///////////////////////////////////////////////////////////////////////////////
#define MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC)                         \
template <typename... Args>                                                 \
auto FUNC(Args&&... args) -> std::enable_if_t<!std::is_void_v<              \
decltype((MODULE().MODULE::FUNC)(std::forward<Args>(args)...))>,            \
decltype((MODULE().MODULE::FUNC)(std::forward<Args>(args)...))>{            \
    constexpr MatmulDfxFuncInfo info{#MODULE, #FUNC, __COUNTER__};          \
    MatmulDfxHandler::PreCall(info, std::forward<Args>(args)...);           \
    auto ret = (M_.MODULE::FUNC)(std::forward<Args>(args)...);              \
    MatmulDfxHandler::PostCall(info, ret);                                  \
    return ret;                                                             \
}                                                                           \
template <typename... Args>                                                 \
auto FUNC(Args&&... args) -> std::enable_if_t<std::is_void_v<               \
decltype((MODULE().MODULE::FUNC)(std::forward<Args>(args)...))>> {          \
    constexpr MatmulDfxFuncInfo info{#MODULE, #FUNC, __COUNTER__};          \
    MatmulDfxHandler::PreCall(info, std::forward<Args>(args)...);           \
    (M_.MODULE::FUNC)(std::forward<Args>(args)...);                         \
    MatmulDfxHandler::PostCall(info);                                       \
}   

///////////////////////////////////////////////////////////////////////////////
#define MATMUL_COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define MATMUL_COUNT_ARGS(...)                                              \
MATMUL_COUNT_ARGS_IMPL(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define MATMUL_DEF_PROXY_FUNC_1(M_, MODULE, FUNC1)                          \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC1)

#define MATMUL_DEF_PROXY_FUNC_2(M_, MODULE, FUNC1, FUNC2)                   \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC1)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC2)

#define MATMUL_DEF_PROXY_FUNC_3(M_, MODULE, FUNC1, FUNC2, FUNC3)            \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC1)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC2)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC3)

#define MATMUL_DEF_PROXY_FUNC_4(M_, MODULE, FUNC1, FUNC2, FUNC3, FUNC4)     \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC1)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC2)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC3)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC4)

#define MATMUL_DEF_PROXY_FUNC_5(M_, MODULE, FUNC1, FUNC2, FUNC3, FUNC4, FUNC5) \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC1)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC2)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC3)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC4)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC5)

#define MATMUL_DEF_PROXY_FUNC_6(M_, MODULE, FUNC1, FUNC2, FUNC3, FUNC4, FUNC5, FUNC6) \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC1)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC2)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC3)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC4)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC5)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC6)

#define MATMUL_DEF_PROXY_FUNC_7(M_, MODULE, FUNC1, FUNC2, FUNC3, FUNC4, FUNC5, FUNC6, FUNC7) \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC1)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC2)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC3)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC4)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC5)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC6)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC7)

#define MATMUL_DEF_PROXY_FUNC_8(M_, MODULE, FUNC1, FUNC2, FUNC3, FUNC4, FUNC5, FUNC6, FUNC7, FUNC8) \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC1)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC2)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC3)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC4)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC5)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC6)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC7)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC8)

#define MATMUL_DEF_PROXY_FUNC_9(M_, MODULE, FUNC1, FUNC2, FUNC3, FUNC4, FUNC5, FUNC6, FUNC7, FUNC8, FUNC9) \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC1)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC2)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC3)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC4)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC5)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC6)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC7)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC8)                                \
MATMUL_DEF_DFX_PROXY_FUNC(M_, MODULE, FUNC9)

#define MATMUL_DEF_DFX_FUNCS_IMPL2(N, M_, MODULE, ...)                      \
MATMUL_DEF_PROXY_FUNC_##N(M_, MODULE, __VA_ARGS__)

#define MATMUL_DEF_DFX_FUNCS_IMPL(N, M_, MODULE, ...)                       \
MATMUL_DEF_DFX_FUNCS_IMPL2(N, M_, MODULE, __VA_ARGS__)

#define MATMUL_DEF_DFX_FUNCS(M_, MODULE, ...)                               \
MATMUL_DEF_DFX_FUNCS_IMPL(MATMUL_COUNT_ARGS(__VA_ARGS__), M_, MODULE, __VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////
#define MATMUL_DFX_PROXY_REGISTER(MODULE, ...)                              \
template <typename T>                                                       \
struct MatmulDfxProxy<T, typename T::MODULE> {                              \
    using MODULE = typename T::MODULE;                                      \
    MatmulDfxProxy(MODULE& module) : proxy{module} {}                       \
    struct FuncProxy {                                                      \
        FuncProxy(MODULE& module) : m_{module} {}                           \
        auto& operator*() { return m_; }                                    \
        MATMUL_DEF_DFX_FUNCS(m_, MODULE, __VA_ARGS__)                       \
    private:                                                                \
        MODULE& m_;                                                         \
    };                                                                      \
    auto operator->() { return &proxy; }                                    \
    operator MODULE*() { return &(*proxy); }                                \
private:                                                                    \
    FuncProxy proxy;                                                        \
};                                                                          \
template <typename T>                                                       \
struct MatmulDfxProxy<const T, typename T::MODULE> {                        \
    using MODULE = typename T::MODULE;                                      \
    MatmulDfxProxy(const MODULE& module) : proxy{module} {}                 \
    struct FuncProxy {                                                      \
        FuncProxy(const MODULE& module) : m_{module} {}                     \
        const auto& operator*() { return m_; }                              \
        MATMUL_DEF_DFX_FUNCS(m_, MODULE, __VA_ARGS__)                       \
    private:                                                                \
        const MODULE& m_;                                                   \
    };                                                                      \
    auto operator->() { return &proxy; }                                    \
    operator MODULE*() { return &(*proxy); }                                \
private:                                                                    \
    FuncProxy proxy;                                                        \
}

}

#endif
