/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MMAD_H
#define MMAD_H

#include "matmul_tensor.h"

namespace matmul {

template<typename IMPL, typename SRC_T, typename DST_T, const auto& MM_CFG>
class MMad {
public:
    void Compute(LocalTensor<DST_T>& DST_Tensor, const LocalTensor<SRC_T>& lhs, LocalTensor<SRC_T>& rhs) {
        for (uint32_t r = 0; r < MM_CFG.basicM; r++) {
            for (uint32_t c = 0; c < MM_CFG.basicN; c++) {
                auto dstAddr = DST_Tensor.GetAddr();
                auto& dstValue = dstAddr[r * MM_CFG.basicN + c];
                for (uint32_t k = 0; k < MM_CFG.basicK; k++) {
                    auto leftAddr = lhs.GetAddr();
                    auto rightAddr = rhs.GetAddr();
                    auto& leftValue = leftAddr[r * MM_CFG.basicK + k];
                    auto& rightValue = rightAddr[k * MM_CFG.basicN + c];
                    dstValue += leftValue * rightValue;
                }
            }
        }
    }
};

}

#endif
