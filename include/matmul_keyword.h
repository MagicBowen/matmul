/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_KEYWORD_H
#define MATMUL_KEYWORD_H

#include <stdint.h>
#include <type_traits>
#include "dfx/matmul_dfx_cfg.h"

/////////////////////////////////////////////////////////////////
#define MATMUL_IMPL__ IMPL

#define MATMUL_CAST_TO_IMPL()                                   \
static_cast<MATMUL_IMPL__*>(this)

#define MATMUL_CAST_TO_CONST_IMPL()                             \
static_cast<const MATMUL_IMPL__*>(this)

#define MATMUL_CAST_TO_IMPL_OF(...)                             \
(static_cast<typename MATMUL_IMPL__::__VA_ARGS__*>(MATMUL_CAST_TO_IMPL()))

#define MATMUL_CAST_TO_CONST_IMPL_OF(...)                       \
(static_cast<const typename MATMUL_IMPL__::__VA_ARGS__*>(MATMUL_CAST_TO_CONST_IMPL()))

#define MATMUL_CAST_TO_PROXY_OF(NAME)                           \
typename MATMUL_IMPL__::template MatmulDfxProxy<typename IMPL::NAME> {*MATMUL_CAST_TO_IMPL_OF(NAME)}; 

#define MATMUL_CAST_TO_CONST_PROXY_OF(NAME)                     \
typename MATMUL_IMPL__::template MatmulDfxProxy<const typename IMPL::NAME> {*MATMUL_CAST_TO_CONST_IMPL_OF(NAME)}; 

/////////////////////////////////////////////////////////////////
#define MATMUL_MODULE(NAME)      cast_to_##NAME()

#define MATMUL_USE_MODULE(NAME)                                 \
inline constexpr decltype(auto) MATMUL_MODULE(NAME) {           \
    if constexpr (MatmulDfxCfg::ENABLE) {                       \
        return MATMUL_CAST_TO_PROXY_OF(NAME);                   \
    } else {                                                    \
        return MATMUL_CAST_TO_IMPL_OF(NAME);                    \
    }                                                           \
}                                                               \
inline constexpr decltype(auto) MATMUL_MODULE(NAME) const {     \
    if constexpr (MatmulDfxCfg::ENABLE) {                       \
        return MATMUL_CAST_TO_CONST_PROXY_OF(NAME);             \
    } else {                                                    \
        return MATMUL_CAST_TO_CONST_IMPL_OF(NAME);              \
    }                                                           \
}

#define MATMUL_USE_MODULE_ON(NAME, ...)                         \
inline constexpr decltype(auto) MATMUL_MODULE(NAME) {           \
    if constexpr (MatmulDfxCfg::ENABLE) {                       \
        return MATMUL_CAST_TO_PROXY_OF(template NAME<__VA_ARGS__>);\
    } else {                                                    \
        return MATMUL_CAST_TO_IMPL_OF(template NAME<__VA_ARGS__>); \
    }                                                           \
}                                                               \
inline constexpr decltype(auto) MATMUL_MODULE(NAME) const {     \
    if constexpr (MatmulDfxCfg::ENABLE) {                       \
        return MATMUL_CAST_TO_CONST_PROXY_OF(template NAME<__VA_ARGS__>);\
    } else {                                                    \
        return MATMUL_CAST_TO_CONST_IMPL_OF(template NAME<__VA_ARGS__>); \
    }                                                           \
}

/////////////////////////////////////////////////////////////////
#define MATMUL_POLICY_TEMPLATE     MATMUL_POLICY

#define MATMUL_POLICY_TEMPLATE_DEFAULT_OF(DEFAULT)              \
template<typename, typename, typename, typename, typename,      \
const auto& = MM_CFG, typename = MM_CB>                         \
class MATMUL_POLICY_TEMPLATE = DEFAULT

#define MATMUL_POLICY_TEMPLATE_OF(NAME)                         \
template<typename, typename, typename, typename, typename, const auto&, typename> class NAME

#define MATMUL_IMPL_TYPE                                        \
MatmulImpl<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, MM_CFG, MM_CB, MATMUL_POLICY_TEMPLATE>

#define MATMUL_MODULE_IN_POLICY(...)                            \
MATMUL_POLICY_TEMPLATE<MATMUL_IMPL_TYPE, A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, MM_CFG, MM_CB>::__VA_ARGS__

#define MATMUL_IMPORT_MODULE(...)  private MATMUL_MODULE_IN_POLICY(__VA_ARGS__)

/////////////////////////////////////////////////////////////////
#define MATMUL_PRIVATE_TEMPLATE    MatmulPrivateModules

#define MATMUL_MODULE_IN_PRIVATE(...)                           \
MATMUL_PRIVATE_TEMPLATE<MATMUL_IMPL_TYPE, A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, MM_CFG>::__VA_ARGS__

#define MATMUL_IMPORT_MODULE_PRIVATE(...)  private MATMUL_MODULE_IN_PRIVATE(__VA_ARGS__)

/////////////////////////////////////////////////////////////////
#define MATMUL_ALLOW_USING(NAME)                                \
friend typename MATMUL_MODULE_IN_POLICY(NAME);                  \
using NAME = typename MATMUL_MODULE_IN_POLICY(NAME)

#define MATMUL_ALLOW_USING_TEMPLATE(NAME, ...)                  \
using NAME = typename MATMUL_MODULE_IN_POLICY(template NAME<__VA_ARGS__>)

/////////////////////////////////////////////////////////////////
#define MATMUL_ALLOW_USING_PRIVATE(NAME)                        \
friend typename MATMUL_MODULE_IN_PRIVATE(NAME);                 \
using NAME = typename MATMUL_MODULE_IN_PRIVATE(NAME)

#define MATMUL_ALLOW_USING_TEMPLATE_PRIVATE(NAME, ...)          \
using NAME = typename MATMUL_MODULE_IN_PRIVATE(template NAME<__VA_ARGS__>)

/////////////////////////////////////////////////////////////////
#define MATMUL_CONTEXT()   MATMUL_CAST_TO_IMPL()->var
#define MATMUL_CONTEXT_CONST() MATMUL_CAST_TO_CONST_IMPL()->var

#define MATMUL_PARAM_VAR   MATMUL_CONTEXT()
#define MATMUL_CONST_PARAM_VAR  MATMUL_CONTEXT_CONST()

#endif
