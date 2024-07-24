/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef SPLIT_LOAD_H
#define SPLIT_LOAD_H

#include "matmul_tensor.h"

namespace matmul {

template<typename TYPE, const auto& MM_CFG>
class SplitLoad {
public:
    LocalTensor<TYPE> Load(LocalTensor<TYPE>&, uint32_t offset);
};

}

#endif
