/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef COPY_CUBE_OUT_H
#define COPY_CUBE_OUT_H

#include "matmul_tensor.h"

namespace matmul {

template<typename IMPL, typename C_TYPE, const auto& MM_CFG>
class CopyCubeOut {

    using DST_T = typename C_TYPE::T;

public:
    void Copy(LocalTensor<DST_T>& dstTensor, const LocalTensor<DST_T>& srcTensor);
    void Copy(GlobalTensor<DST_T>& dstTensor, const LocalTensor<DST_T>& srcTensor) {
        auto dstAddr = dstTensor.GetAddr();
        auto srcAddr = srcTensor.GetAddr();
        for (uint32_t i = 0; i < MM_CFG.basicM; i++) {
            for (uint32_t j = 0; j < MM_CFG.basicN; j++) {
                dstAddr[i * MM_CFG.basicN + j] = srcAddr[i * MM_CFG.basicN + j];
            }
        }
    }
};

}

#endif
