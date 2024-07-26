/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef COPY_IN_BUFFER_H
#define COPY_IN_BUFFER_H

#include "modules/input_type_traits.h"
#include "matmul_tensor.h"

namespace matmul {

template<typename IMPL, typename INPUT_TYPE, const auto& MM_CFG>
class CopyInBuffer {

    using SrcT = typename INPUT_TYPE::INNER_T;
    static constexpr uint32_t BLOCK_SIZE = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetBlockSize();

public:
    void Init(uint32_t blockNum, uint32_t capacity) {
        this->blockNum = blockNum;
        this->capacity = capacity;
    }

    void Destroy() {
    }

    LocalTensor<SrcT> AllocTensor(uint32_t row, uint32_t col) {
        LocalTensor<SrcT> tensor;
        return tensor;
    }

    void FreeTensor(LocalTensor<SrcT>& tensor) {
    }

    LocalTensor<SrcT> GetTensor() {
        LocalTensor<SrcT> tensor;
        return tensor;
    }

private:
    uint32_t blockNum{0};
    uint32_t capacity{0};
};

}

#endif
