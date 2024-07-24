/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_KEYWORD_H
#define MATMUL_KEYWORD_H

#include <stdint.h>
#include <type_traits>

#define MATMUL_IMPL_ IMPL

#define MATMUL_CAST_TO_IMPL()                                   \
static_cast<MATMUL_IMPL_*>(this)

#define MATMUL_CAST_TO(...)                                    \
(*(static_cast<typename MATMUL_IMPL_::__VA_ARGS__*>(MATMUL_CAST_TO_IMPL())))

#define MATMUL_MODULE(NAME)      cast_to_##NAME()

#define MATMUL_USE_MODULE(NAME)                                 \
inline constexpr auto MATMUL_MODULE(NAME) -> decltype(auto) {   \
    return MATMUL_CAST_TO(NAME);                                \
}

#define MATMUL_USE_TEMPLATE_MODULE(NAME, ...)                   \
inline constexpr auto MATMUL_MODULE(NAME) -> decltype(auto) {   \
    return MATMUL_CAST_TO(template NAME<__VA_ARGS__>);          \
}

#define MATMUL_POLICY_TEMPLATE     MATMUL_POLICY

#define MATMUL_POLICY_TEMPLATE_DEFAULT_OF(DEFAULT)              \
template<typename, typename, typename, typename, typename,      \
const auto& = MM_CFG, typename = MM_CB>                         \
class MATMUL_POLICY_TEMPLATE = DEFAULT

#define MATMUL_POLICY_TEMPLATE_OF(NAME)                         \
template<typename, typename, typename, typename, typename, const auto&, typename> class NAME

#define MATMUL_IMPL_TYPE                                        \
MatmulImpl<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, MM_CFG, MM_CB, MATMUL_POLICY_TEMPLATE>

#define MATMUL_MODULE_IN_POLICY(...)                           \
MATMUL_POLICY_TEMPLATE<MATMUL_IMPL_TYPE, A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, MM_CFG, MM_CB>::__VA_ARGS__

#define MATMUL_IMPORT_MODULE(...)  private MATMUL_MODULE_IN_POLICY(__VA_ARGS__)

#define MATMUL_ALLOW_USING(NAME)                                \
friend typename MATMUL_MODULE_IN_POLICY(NAME);                  \
using NAME = typename MATMUL_MODULE_IN_POLICY(NAME)

#define MATMUL_ALLOW_USING_TEMPLATE(NAME, ...)                  \
using NAME = typename MATMUL_MODULE_IN_POLICY(template NAME<__VA_ARGS__>)

#define MATMUL_CONTEXT()   MATMUL_CAST_TO_IMPL()->var
#define MATMUL_CONTEXT_CONST() ((const MATMUL_IMPL_*)(this))->var

#define MATMUL_PARAM_VAR   MATMUL_CONTEXT()
#define MATMUL_CONST_PARAM_VAR  MATMUL_CONTEXT_CONST()

#endif
