/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef COPY_CUBE_IN_H
#define COPY_CUBE_IN_H

#include "matmul_tensor.h"

namespace matmul {

template<typename TYPE, const auto& MM_CFG>
class CopyCubeIn {
public:
    void Copy(LocalTensor<Type>&, uint32_t row, uint32_t col, uint32_t height, uint32_t width);
};

}

#endif
