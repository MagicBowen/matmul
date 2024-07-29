/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MMAD_H
#define MMAD_H

#include "matmul_tensor.h"

namespace matmul {

template<typename IMPL, typename ST, typename DT, const auto& MM_CFG>
class MMad {
public:
    void Compute(LocalTensor<DT>& dstTensor, const LocalTensor<ST>& lhs, LocalTensor<ST>& rhs) {
        for (uint32_t r = 0; r < MM_CFG.basicM; r++) {
            for (uint32_t c = 0; c < MM_CFG.basicN; c++) {
                auto dstAddr = dstTensor.GetAddr();
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
