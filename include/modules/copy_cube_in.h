/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef COPY_CUBE_IN_H
#define COPY_CUBE_IN_H

#include "matmul_tensor.h"
#include "matmul_keyword.h"
#include "modules/input_type_traits.h"
#include "modules/copy_in_buffer.h"
#include "modules/hal_instruction.h"

namespace matmul {

/////////////////////////////////////////////////////////////////
template<typename IMPL, typename INPUT_TYPE, const auto& MM_CFG, typename = void>
class CopyCubeIn {

    MATMUL_USE_MODULE_ON(CopyInBuffer, INPUT_TYPE::TAG);
    MATMUL_USE_MODULE(HalInstruction);

    using SrcT = typename INPUT_TYPE::INNER_T;

public:
    void Init() {
        MATMUL_MODULE(CopyInBuffer)->Init(L1_LOAD_SIZE, L1_BUFFER_NUM);
        std::cout << "CopyCubeIn::Init, in common version" << std::endl;
    }

    void Destroy() {
        MATMUL_MODULE(CopyInBuffer)->Destroy();
    }

    void SetAddr(const GlobalTensor<SrcT>& input) {
        addr = const_cast<SrcT*>(input.GetAddr());
    }

    void SetAddr(const LocalTensor<SrcT>& input) {
    }

    LocalTensor<SrcT> Load(uint32_t row, uint32_t col) {
        uint32_t stepIdx = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetBufferStepIndex(row, col);
        if (stepIdx == 0) {
            if (cacheTensor.GetAddr()) {
                MATMUL_MODULE(CopyInBuffer)->Free(cacheTensor);
            }
            cacheTensor = MATMUL_MODULE(CopyInBuffer)->Alloc();

            GlobalTensor<SrcT> srcTensor;
            srcTensor.SetAddr(addr + InputTypeTraits<INPUT_TYPE, MM_CFG>::GetOffsetFromOrigin(row, col), L1_LOAD_SIZE);

            MATMUL_MODULE(HalInstruction)->CopyND2NZ(cacheTensor, srcTensor, L1_LOAD_SIZE);
        } 
        
        LocalTensor<SrcT> result;
        result.SetAddr(cacheTensor.GetAddr() + stepIdx * ONE_STEP_CACHE_SIZE, ONE_STEP_CACHE_SIZE);
        return result;
    }

    void Clear(LocalTensor<SrcT>& tensor) {
        MATMUL_MODULE(CopyInBuffer)->Free(tensor);
    }

private:
    SrcT* addr{nullptr};
    LocalTensor<SrcT> cacheTensor;

private:
    static constexpr uint32_t L1_LOAD_SIZE = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetLoadSize();
    static constexpr uint32_t ONE_STEP_CACHE_SIZE = InputTypeTraits<INPUT_TYPE, MM_CFG>::GetOneStepSize();
    static constexpr uint32_t L1_BUFFER_NUM = 2;
};

/////////////////////////////////////////////////////////////////
template<typename IMPL, typename INPUT_TYPE, const auto& MM_CFG>
class CopyCubeIn<IMPL, INPUT_TYPE, MM_CFG, std::enable_if_t<PhyPosIsL1(INPUT_TYPE::pos)>> {

    using SrcT = typename INPUT_TYPE::INNER_T;

public:
    void Init() {
        std::cout << "CopyCubeIn::Init, in partial specialized version" << std::endl;
    }

    void Destroy() {
    }

    void SetAddr(const GlobalTensor<SrcT>& input) {
    }

    void SetAddr(const LocalTensor<SrcT>& input) {
        addr = input.GetAddr();
    }

    LocalTensor<SrcT>& Load(uint32_t row, uint32_t col) {
        LocalTensor<SrcT> tensor;
        std::cout << "CopyCubeIn::Load, in partial specialized version" << std::endl;
        return tensor;
    }

    void Clear(LocalTensor<SrcT>&) {
    }

private:
    const SrcT* addr{nullptr};
};

}

#endif
