/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef COPY_IN_BUFFER_H
#define COPY_IN_BUFFER_H

#include "matmul_tensor.h"
#include "modules/input_type_traits.h"

namespace matmul {

template<typename IMPL, typename INPUT_TYPE, const auto& MM_CFG>
class CopyInBuffer {

    using SrcT = typename INPUT_TYPE::INNER_T;
    static constexpr uint32_t BLOCK_SIZE = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetBlockSize();

public:
    void Init() {
        Clear();
    }

    LocalTensor<SrcT> AllocTensor() {
        LocalTensor<SrcT> tensor;
        tensor.SetAddr(addr, BLOCK_SIZE);
        return tensor;
    }

    void FreeTensor(LocalTensor<SrcT>&) {
        Clear();
    }

private:
    void Clear() {
        memset(addr, 0, sizeof(SrcT) * BLOCK_SIZE);
    }

private:
    SrcT* addr[BLOCK_SIZE];
};

}

#endif
