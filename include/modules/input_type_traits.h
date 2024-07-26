/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef INPUT_TYPE_TRAITS_H
#define INPUT_TYPE_TRAITS_H

#include "matmul_type_def.h"

namespace matmul {

enum class InputTypeTag {
    A, B
};

template <typename INPUT_TYPE, typename INNER_TYPE = typename INPUT_TYPE::T>
struct MatmulInputTypeA : INPUT_TYPE {
    using INNER_T = INNER_TYPE;
    constexpr static InputTypeTag TAG = InputTypeTag::A;
};

template <typename INPUT_TYPE, typename INNER_TYPE = typename INPUT_TYPE::T>
struct MatmulInputTypeB : INPUT_TYPE {
    using INNER_T = INNER_TYPE;
    constexpr static InputTypeTag TAG = InputTypeTag::B;
};

template<typename INPUT_TYPE, const auto& MM_CFG>
class InputTypeTraits {
public:
    static constexpr uint32_t GetRowSize();
    static constexpr uint32_t GetBasicRowSize();
    static constexpr uint32_t GetColSize();
    static constexpr uint32_t GetBasicColSize();
    static constexpr uint32_t GetBlockSize();
};

template<typename INPUT_TYPE, typename INNER_TYPE, const auto& MM_CFG>
class InputTypeTraits<MatmulInputTypeA<INPUT_TYPE, INNER_TYPE>, MM_CFG> {
public:
    static constexpr uint32_t GetRowSize() {
        return MM_CFG.singleCoreK;
    }
    static constexpr uint32_t GetBasicRowSize() {
        return MM_CFG.basicM;
    }
    static constexpr uint32_t GetColSize() {
        return MM_CFG.singleCoreM;
    }
    static constexpr uint32_t GetBasicColSize() {
        return MM_CFG.basicK;
    }

    static constexpr uint32_t GetBlockSize() {
        return GetBasicRowSize() * GetBasicColSize();   
    }
};

template<typename INPUT_TYPE, typename INNER_TYPE, const auto& MM_CFG>
class InputTypeTraits<MatmulInputTypeB<INPUT_TYPE, INNER_TYPE>, MM_CFG> {
public:
    static constexpr uint32_t GetRowSize() {
        return MM_CFG.singleCoreN;
    }
    static constexpr uint32_t GetBasicRowSize() {
        return MM_CFG.basicK;
    }
    static constexpr uint32_t GetColSize() {
        return MM_CFG.singleCoreK;
    }
    static constexpr uint32_t GetBasicColSize() {
        return MM_CFG.basicN;
    }

    static constexpr uint32_t GetBlockSize() {
        return GetBasicRowSize() * GetBasicColSize();   
    }
};

}

#endif
