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

template<typename A_TYPE, typename B_TYPE>
struct SrcType {
    using Type = typename A_TYPE::INNER_T;
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
struct InputTypeTraits {
    static constexpr uint32_t GetRowSize();
    static constexpr uint32_t GetColSize();
    static constexpr uint32_t GetRowBytes();
    static constexpr uint32_t GetColBytes();
    static constexpr uint32_t GetBasicRowSize();
    static constexpr uint32_t GetBasicColSize();
    static constexpr uint32_t GetBlockSize();
    static constexpr uint32_t GetBlockBytes();
    static constexpr uint32_t GetL1LoadSize();
};

template<typename INPUT_TYPE, typename INNER_TYPE, const auto& MM_CFG>
struct InputTypeTraits<MatmulInputTypeA<INPUT_TYPE, INNER_TYPE>, MM_CFG> {
    static constexpr uint32_t GetRowSize() {
        return MM_CFG.singleCoreK;
    }
    static constexpr uint32_t GetRowBytes() {
        return GetRowSize() * sizeof(INNER_TYPE);
    }
    static constexpr uint32_t GetColSize() {
        return MM_CFG.singleCoreM;
    }
    static constexpr uint32_t GetColBytes() {
        return GetColSize() * sizeof(INNER_TYPE);
    }
    static constexpr uint32_t GetBasicRowSize() {
        return MM_CFG.basicM;
    }
    static constexpr uint32_t GetBasicColSize() {
        return MM_CFG.basicK;
    }
    static constexpr uint32_t GetBlockSize() {
        return GetBasicRowSize() * GetBasicColSize();   
    }
    static constexpr uint32_t GetBlockBytes() {
        return GetBlockSize() * sizeof(INNER_TYPE);   
    }
    static constexpr uint32_t GetL1LoadSize() {
        return MM_CFG.stepM * (MM_CFG.singleCoreK / MM_CFG.basicK);
    }
};

template<typename INPUT_TYPE, typename INNER_TYPE, const auto& MM_CFG>
struct InputTypeTraits<MatmulInputTypeB<INPUT_TYPE, INNER_TYPE>, MM_CFG> {
    static constexpr uint32_t GetRowSize() {
        return MM_CFG.singleCoreN;
    }
    static constexpr uint32_t GetRowBytes() {
        return GetRowSize() * sizeof(INNER_TYPE);
    }
    static constexpr uint32_t GetColSize() {
        return MM_CFG.singleCoreK;
    }
    static constexpr uint32_t GetColBytes() {
        return GetColSize() * sizeof(INNER_TYPE);
    }
    static constexpr uint32_t GetBasicRowSize() {
        return MM_CFG.basicK;
    }
    static constexpr uint32_t GetBasicColSize() {
        return MM_CFG.basicN;
    }
    static constexpr uint32_t GetBlockSize() {
        return GetBasicRowSize() * GetBasicColSize();   
    }
    static constexpr uint32_t GetBlockBytes() {
        return GetBlockSize() * sizeof(INNER_TYPE);   
    }
    static constexpr uint32_t GetL1LoadSize() {
        return MM_CFG.stepN * (MM_CFG.singleCoreK / MM_CFG.basicK);
    }
};

}

#endif
