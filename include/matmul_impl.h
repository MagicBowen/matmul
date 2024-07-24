/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_IMPL_H
#define MATMUL_IMPL_H

#include "matmul_tiling.h"
#include "matmul_params.h"
#include "matmul_type_def.h"
#include "matmul_config.h"
#include "matmul_call_back.h"
#include "matmul_policy.h"

namespace matmul {

template <typename A_TYPE, typename B_TYPE, typename C_TYPE, typename BIAS_TYPE, 
          const auto& MM_CFG = CFG_NORM, typename MM_CB = MatmulCallBackFunc<nullptr, nullptr, nullptr>, 
          MATMUL_POLICY_TEMPLATE_DEFAULT_OF(MatmulPolicyDefault)>
class MatmulImpl
: MATMUL_IMPORT_MODULE(Scheduler)
, MATMUL_IMPORT_MODULE(IterateController)
, MATMUL_IMPORT_MODULE(CopyCubeInA)
, MATMUL_IMPORT_MODULE(CopyCubeInB)
, MATMUL_IMPORT_MODULE(CopyInBufferA)
, MATMUL_IMPORT_MODULE(CopyInBufferB)
, MATMUL_IMPORT_MODULE(MMad)
{
    using L0cT = typename A_TYPE::T;
    using SrcT = typename A_TYPE::T;
    using SrcAT = typename A_TYPE::T;
    using SrcBT = typename B_TYPE::T;
    using DstT = typename C_TYPE::T;
    using BiasT = typename BIAS_TYPE::T;

public:
    MATMUL_ALLOW_USING(Scheduler);
    MATMUL_ALLOW_USING(IterateController);
    MATMUL_ALLOW_USING(CopyCubeInA);
    MATMUL_ALLOW_USING(CopyCubeInB);
    MATMUL_ALLOW_USING(CopyInBufferA);
    MATMUL_ALLOW_USING(CopyInBufferB);
    MATMUL_ALLOW_USING(MMad);

    template<InputTag TAG>
    using CopyInBuffer = std::conditional_t<TAG == InputTag::LEFT, CopyInBufferA, CopyInBufferB>;

    // template<typename TYPE, InputTag TAG>
    // MATMUL_ALLOW_USING_TEMPLATE(CopyInBuffer, TYPE, TAG);

    // template<typename IMPL, typename INPUT_TYPE, const auto& MM_CFG_, InputTag TAG>
    // friend class matmul::CopyInBuffer;

    // template<typename TYPE, InputTag TAG>
    // friend class MATMUL_POLICY<MatmulImpl<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, MM_CFG, MM_CB, MATMUL_POLICY>, A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, MM_CFG, MM_CB>::CopyInBuffer;


public:
    void Init(const TCubeTiling* tiling) {
        var.tiling = tiling;
        var.singleCoreM_ = MM_CFG.singleCoreM;
        var.singleCoreN_ = MM_CFG.singleCoreN;
        var.singleCoreK_ = MM_CFG.singleCoreK;
        var.mIter = MM_CFG.singleCoreM / MM_CFG.basicM;
        var.nIter = MM_CFG.singleCoreN / MM_CFG.basicN;
        var.kIter = MM_CFG.singleCoreK / MM_CFG.basicK;
    }

    void SetTensorA(const GlobalTensor<SrcAT>& leftMatrix) {
        CopyCubeInA::SetAddr(leftMatrix);
    }

    void SetTensorA(const LocalTensor<SrcAT>& leftMatrix) {
        CopyCubeInA::SetAddr(leftMatrix);
    }

    void SetTensorB(const GlobalTensor<SrcBT>& rightMatrix) {
        CopyCubeInB::SetAddr(rightMatrix);
    }

    void SetTensorB(const LocalTensor<SrcBT>& rightMatrix) {
        CopyCubeInB::SetAddr(rightMatrix);
    }

    void SetBias(const GlobalTensor<BiasT>& bias) {
    }

    void SetBias(const LocalTensor<BiasT>& bias) {
    }

    void GetTensorC(const LocalTensor<DstT>& tensor) {
    }

    void GetTensorC(const GlobalTensor<DstT>& tensor) {
    }

    bool Iterate() {
        return false;
    }

    void IterateAll(const GlobalTensor<DstT>&) {

    }

    void IterateAll(const LocalTensor<DstT>&) {
        Scheduler::Schedule();
    }

private:
    MatmulParams var;
};

}

#endif
