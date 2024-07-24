/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef COPY_IN_BUFFER_H
#define COPY_IN_BUFFER_H

#include "matmul_tensor.h"
#include "modules/input_tag.h"

namespace matmul {

template<typename IMPL, typename TYPE, const auto& MM_CFG, InputTag TAG>
class CopyInBuffer {
    static constexpr uint32_t BLOCK_SIZE = InputTraits<MM_CFG, TAG>::GetBlockSize();
public:
    void Init() {
        Clear();

        printf("%d\n", MATMUL_CONTEXT().singleCoreM_);
    }

    LocalTensor<TYPE> AllocTensor() {
        LocalTensor<TYPE> tensor;
        tensor.SetAddr(addr, BLOCK_SIZE);
        return tensor;
    }

    void FreeTensor(LocalTensor<TYPE>&) {
        Clear();
    }

private:
    void Clear() {
        memset(addr, 0, sizeof(TYPE) * BLOCK_SIZE);
    }

private:
    TYPE* addr[BLOCK_SIZE];
};

}

#endif
