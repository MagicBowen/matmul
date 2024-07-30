/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef COPY_CUBE_IN_H
#define COPY_CUBE_IN_H

#include "matmul_tensor.h"
#include "matmul_keyword.h"
#include "modules/input_type_traits.h"
#include "modules/copy_in_buffer.h"

namespace matmul {

/////////////////////////////////////////////////////////////////
template<typename IMPL, typename INPUT_TYPE, const auto& MM_CFG, typename = void>
class CopyCubeIn {

    MATMUL_USE_MODULE_ON(CopyInBuffer, INPUT_TYPE::TAG);

    using SrcT = typename INPUT_TYPE::INNER_T;

public:
    void Init() {
        MATMUL_MODULE(CopyInBuffer)->Init(L1_LOAD_NUM, 1);
    }

    void Destroy() {
        MATMUL_MODULE(CopyInBuffer)->Destroy();
    }

    void SetAddr(const GlobalTensor<SrcT>& input) {
        addr = input.GetAddr();
    }

    void SetAddr(const LocalTensor<SrcT>& input) {
    }

    LocalTensor<SrcT> Load(uint32_t row, uint32_t col) {
        auto tensor = MATMUL_MODULE(CopyInBuffer)->Alloc(row, col);
        copy(tensor, row, col);
        return tensor;
    }

    void Clear(LocalTensor<SrcT>& tensor) {
        MATMUL_MODULE(CopyInBuffer)->Free(tensor);
    }

private:
    void copy(LocalTensor<SrcT>& tensor, uint32_t row, uint32_t col) {
        // auto dstAddr = tensor.GetAddr();
        // for (uint32_t h = 0; h < height; h++) {
        //     for (uint32_t w = 0; w < width; w++) {
        //         for (uint32_t r = 0; r < BASIC_ROW_SIZE; r++) {
        //             uint32_t srcPos = ((row + h) * BASIC_ROW_SIZE + r) * ROW_SIZE + (col + w) * BASIC_COL_SIZE;
        //             uint32_t dstPos = (h * width + w) * BLOCK_SIZE;
        //             memcpy(&dstAddr[dstPos + r * BASIC_COL_SIZE], &addr[srcPos], BASIC_COL_SIZE * sizeof(SrcT));
        //         }
        //     }
        // }
    }

private:
    const SrcT* addr{nullptr};

private:
    static constexpr uint32_t L1_LOAD_NUM = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetL1LoadNum();

    static constexpr uint32_t BLOCK_SIZE = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetBlockSize();
    static constexpr uint32_t BASIC_ROW_SIZE = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetBasicRowSize();
    static constexpr uint32_t BASIC_COL_SIZE = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetBasicColSize();
    static constexpr uint32_t ROW_SIZE = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetRowSize();
    static constexpr uint32_t COL_SIZE = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetColSize();
};

/////////////////////////////////////////////////////////////////
template<typename IMPL, typename INPUT_TYPE, const auto& MM_CFG>
class CopyCubeIn<IMPL, INPUT_TYPE, MM_CFG, std::enable_if_t<PhyPosIsL1(INPUT_TYPE::Pos)>> {

    using SrcT = typename INPUT_TYPE::INNER_T;

public:
    void Init() {
    }

    void SetAddr(const GlobalTensor<SrcT>& input) {
    }

    void SetAddr(const LocalTensor<SrcT>& input) {
        addr = input.GetAddr();
    }

    LocalTensor<SrcT>& Load(uint32_t row, uint32_t col) {
    }

    void Clear(LocalTensor<SrcT>&) {
    }

private:
    const SrcT* addr{nullptr};
};

}

#endif
