/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef SPLIT_LOAD_H
#define SPLIT_LOAD_H

#include "matmul_tensor.h"

namespace matmul {

template<typename INPUT_TYPE, const auto& MM_CFG>
class SplitLoad {
    using SrcT = typename INPUT_TYPE::INNER_T;
public:
    LocalTensor<SrcT> Split(LocalTensor<SrcT>& input, uint32_t offset) {
        return input;
    }
};

}

#endif
