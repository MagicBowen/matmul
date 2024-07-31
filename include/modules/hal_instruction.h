/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef HAL_INSTRUCTION_H
#define HAL_INSTRUCTION_H

namespace matmul {

template<typename IMPL, const auto& MM_CFG>
class HalInstruction {
public:
    template<typename DST_TENSOR, typename SRC_TENSOR>
    void CopyND2NZ(DST_TENSOR& dstTensor, const SRC_TENSOR& srcTensor, uint32_t size) {
    }
};

}

#endif
