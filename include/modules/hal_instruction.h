/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef HAL_INSTRUCTION_H
#define HAL_INSTRUCTION_H

namespace matmul {

template<typename IMPL, const auto& MM_CFG>
class HalInstruction {
public:
    void CopyND2NZ() {
       // auto dstAddr = tensor.GetAddr();
        // for (uint32_t h = 0; h < height; h++) {
        //     for (uint32_t w = 0; w < width; w++) {
        //         for (uint32_t r = 0; r < BASIC_ROW_SIZE; r++) {
        //             uint32_t srcPos = ((row + h) * BASIC_ROW_SIZE + r) * ROW_SIZE + (col + w) * BASIC_COL_SIZE;
        //             uint32_t dstPos = (h * width + w) * BLOCK_SIZE;
        //             memcpy(&dstAddr[dstPos + r * BASIC_COL_SIZE], &addr[srcPos], BASIC_COL_SIZE * sizeof(SrcT));
        //         }
        //     }
        // }
    }
};

}

#endif
