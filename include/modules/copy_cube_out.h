/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef COPY_CUBE_OUT_H
#define COPY_CUBE_OUT_H

#include "matmul_tensor.h"

namespace matmul {

template<typename TYPE, const auto& MM_CFG>
class CopyCubeOut {
public:
    void Copy(LocalTensor<TYPE>& dstTensor, const LocalTensor<TYPE>& srcTensor);
};

}

#endif
