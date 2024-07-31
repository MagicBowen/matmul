/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef COPY_IN_BUFFER_H
#define COPY_IN_BUFFER_H

#include "modules/input_type_traits.h"
#include "matmul_tensor.h"
#include <vector>

namespace matmul {

template<typename IMPL, typename INPUT_TYPE, const auto& MM_CFG>
class CopyInBuffer {

    using SrcT = typename INPUT_TYPE::INNER_T;
    static constexpr uint32_t BLOCK_SIZE = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetBlockSize();

public:
    void Init(uint32_t bufferSize, uint32_t bufferNum) {
        bufferSize_ = bufferSize;
        bufferNum_ = bufferNum;
    }

    void Destroy() {
    }

    LocalTensor<SrcT> Alloc() {
        LocalTensor<SrcT> tensor;
        return tensor;
    }

    void Free(LocalTensor<SrcT>& tensor) {
    }

    LocalTensor<SrcT> Get() {
        LocalTensor<SrcT> tensor;
        return tensor;
    }

private:
    uint32_t bufferSize_{0};
    uint32_t bufferNum_{0};
};

}

#endif
