/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef COPY_IN_BUFFER_H
#define COPY_IN_BUFFER_H

#include "matmul_tensor.h"

namespace matmul {

template<typename TYPE, const auto& MM_CFG>
class CopyInBuffer {
public:
    LocalTensor<TYPE> AllocTensor();
    void FreeTensor(LocalTensor<TYPE>&);
};

}

#endif
