/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef COPY_CUBE_IN_H
#define COPY_CUBE_IN_H

#include "matmul_tensor.h"
#include "matmul_keyword.h"
#include "modules/input_tag.h"
#include "modules/copy_in_buffer.h"

namespace matmul {

template<typename IMPL, typename TYPE, const auto& MM_CFG, InputTag TAG, typename = void>
class CopyCubeIn {
    static constexpr uint32_t BLOCK_SIZE = InputTraits<MM_CFG, TAG>::GetBlockSize();
    static constexpr uint32_t BASIC_ROW_SIZE = InputTraits<MM_CFG, TAG>::GetBasicRowSize();
    static constexpr uint32_t BASIC_COL_SIZE = InputTraits<MM_CFG, TAG>::GetBasicColSize();
    static constexpr uint32_t ROW_SIZE = InputTraits<MM_CFG, TAG>::GetRowSize();
    static constexpr uint32_t COL_SIZE = InputTraits<MM_CFG, TAG>::GetColSize();

    MATMUL_USE_MODULE_ON(CopyInBuffer, TAG);

public:
    void SetAddr(const GlobalTensor<typename TYPE::T>& input) {
        addr = input.GetAddr();
        MATMUL_MODULE(CopyInBuffer)->Init();
    }

    void Copy(LocalTensor<typename TYPE::T>& tensor, uint32_t row, uint32_t col, uint32_t height, uint32_t width) {
        auto dstAddr = tensor.GetAddr();
        for (uint32_t h = 0; h < height; h++) {
            for (uint32_t w = 0; w < width; w++) {
                for (uint32_t r = 0; r < BASIC_ROW_SIZE; r++) {
                    uint32_t srcPos = ((row + h) * BASIC_ROW_SIZE + r) * ROW_SIZE + (col + w) * BASIC_COL_SIZE;
                    uint32_t dstPos = (h * width + w) * BLOCK_SIZE;
                    memcpy(&dstAddr[dstPos + r * BASIC_COL_SIZE], &addr[srcPos], BASIC_COL_SIZE * sizeof(TYPE::T));
                }
            }
        }
    }

private:
    const typename TYPE::T* addr{nullptr};
};

template<typename IMPL, typename TYPE, const auto& MM_CFG, InputTag TAG>
class CopyCubeIn<IMPL, TYPE, MM_CFG, TAG, std::enable_if_t<PhyPosIsL1(TYPE::Pos)>> {
public:
    void SetAddr(const GlobalTensor<typename TYPE::T>& input) {
    }

    void Copy(LocalTensor<typename TYPE::T>& tensor, uint32_t row, uint32_t col, uint32_t height, uint32_t width) {
    }
};

}

#endif
