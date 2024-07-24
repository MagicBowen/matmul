/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef INPUT_TRAITS_H
#define INPUT_TRAITS_H

namespace matmul {

enum class InputTag {
    LEFT,
    RIGHT,
};

template<const auto& MM_CFG, InputTag TAG>
class InputTraits {
public:
    static constexpr uint32_t GetRowSize();
    static constexpr uint32_t GetBasicRowSize();
    static constexpr uint32_t GetColSize();
    static constexpr uint32_t GetBasicColSize();
    static constexpr uint32_t GetBlockSize();
};

template<const auto& MM_CFG>
class InputTraits<MM_CFG, InputTag::LEFT> {
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

template<const auto& MM_CFG>
class InputTraits<MM_CFG, InputTag::RIGHT> {
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
