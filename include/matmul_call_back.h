/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef LIB_MATMUL_MATMUL_CALL_BACK_H
#define LIB_MATMUL_MATMUL_CALL_BACK_H

#include "matmul_tensor.h"

namespace matmul {
template <void (*DataCopyOut)(const void* gm, const LocalTensor<int8_t> &co1Local,
        const void *dataCopyOutParams, const uint64_t tilingPtr, const uint64_t dataPtr) = nullptr,
        void (*CopyA1)(const LocalTensor<int8_t> &aMatrix, const void *gm, int row, int col, int useM, int useK,
        const uint64_t tilingPtr, const uint64_t dataPtr) = nullptr,
        void (*CopyB1)(const LocalTensor<int8_t> &bMatrix, const void *gm, int row, int col, int useK, int useN,
        const uint64_t tilingPtr, const uint64_t dataPtr) = nullptr>
struct MatmulCallBackFunc {
    constexpr static void (*DataCopyOutPtr)(const void* gm, const LocalTensor<int8_t> &co1Local,
        const void *dataCopyOutParams, const uint64_t tilingPtr, const uint64_t dataPtr) = DataCopyOut;
    constexpr static void (*CopyA1Ptr)(const LocalTensor<int8_t> &aMatrix, const void *gm, int row, int col,
        int useM, int useK, const uint64_t tilingPtr, const uint64_t dataPtr) = CopyA1;
    constexpr static void (*CopyB1Ptr)(const LocalTensor<int8_t> &bMatrix, const void *gm, int row, int col,
        int useK, int useN, const uint64_t tilingPtr, const uint64_t dataPtr) = CopyB1;
};

} // namespace matmul
#endif
