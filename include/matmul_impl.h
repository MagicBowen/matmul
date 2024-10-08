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
#include "matmul_private.h"

namespace matmul {

template <typename A_TYPE, 
          typename B_TYPE, 
          typename C_TYPE, 
          typename BIAS_TYPE, 
          const auto& MM_CFG = CFG_NORM, 
          template<const auto& = MM_CFG, typename...> class POLICY = MatmulPolicyDefault>
class MatmulImpl
: MATMUL_IMPORT_MODULE(Scheduler)
, MATMUL_IMPORT_MODULE(IterateController)
, MATMUL_IMPORT_MODULE(Iterator)
, MATMUL_IMPORT_MODULE(CopyCubeInA)
, MATMUL_IMPORT_MODULE(CopyCubeInB)
, MATMUL_IMPORT_MODULE(CopyInBufferA)
, MATMUL_IMPORT_MODULE(CopyInBufferB)
, MATMUL_IMPORT_MODULE(SplitLoadA)
, MATMUL_IMPORT_MODULE(SplitLoadB)
, MATMUL_IMPORT_MODULE(MMad)
, MATMUL_IMPORT_MODULE(Co1Buffer)
, MATMUL_IMPORT_MODULE(CopyCubeOut)
, MATMUL_IMPORT_MODULE(Context)
, MATMUL_IMPORT_MODULE_PRIVATE(HalInstruction)
{
    using L0cT = typename A_TYPE::T;
    using SrcT = typename A_TYPE::T;
    using SrcAT = typename A_TYPE::T;
    using SrcBT = typename B_TYPE::T;
    using DstT = typename C_TYPE::T;
    using BiasT = typename BIAS_TYPE::T;

private:
    MATMUL_ALLOW_USING(Scheduler);
    MATMUL_ALLOW_USING(IterateController);
    MATMUL_ALLOW_USING(Iterator);
    MATMUL_ALLOW_USING(CopyCubeInA);
    MATMUL_ALLOW_USING(CopyCubeInB);
    MATMUL_ALLOW_USING(CopyInBufferA);
    MATMUL_ALLOW_USING(CopyInBufferB);
    MATMUL_ALLOW_USING(SplitLoadA);
    MATMUL_ALLOW_USING(SplitLoadB);
    MATMUL_ALLOW_USING(MMad);
    MATMUL_ALLOW_USING(Co1Buffer);
    MATMUL_ALLOW_USING(CopyCubeOut);
    MATMUL_ALLOW_USING(Context);

    template<InputTypeTag TAG>
    using CopyInBuffer = std::conditional_t<TAG == InputTypeTag::A, CopyInBufferA, CopyInBufferB>;

private:
    template<typename, typename> friend struct DfxProxy;

private:
    MATMUL_ALLOW_USING_PRIVATE(HalInstruction);

private:
    using IMPL = MATMUL_IMPL_TYPE;
    MATMUL_USE_MODULE(CopyCubeInA);
    MATMUL_USE_MODULE(CopyCubeInB);
    MATMUL_USE_MODULE(Scheduler);

public:
    void Init(const TCubeTiling* tiling) {
        InitContext(tiling);
        MATMUL_MODULE(Scheduler)->Init(tiling);
    }

    void End() {
        MATMUL_MODULE(Scheduler)->Destroy();
    }

    void SetTensorA(const GlobalTensor<SrcAT>& leftMatrix) {
        MATMUL_MODULE(CopyCubeInA)->SetAddr(leftMatrix);
    }

    void SetTensorA(const LocalTensor<SrcAT>& leftMatrix) {
        MATMUL_MODULE(CopyCubeInA)->SetAddr(leftMatrix);
    }

    void SetTensorB(const GlobalTensor<SrcBT>& rightMatrix) {
        MATMUL_MODULE(CopyCubeInB)->SetAddr(rightMatrix);
    }

    void SetTensorB(const LocalTensor<SrcBT>& rightMatrix) {
        MATMUL_MODULE(CopyCubeInB)->SetAddr(rightMatrix);
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
        return MATMUL_MODULE(Scheduler)->Schedule();
    }

    void IterateAll(GlobalTensor<DstT>& tensor) {
        return MATMUL_MODULE(Scheduler)->ScheduleAll(tensor);
    }

    void IterateAll(LocalTensor<DstT>& tensor) {
        return MATMUL_MODULE(Scheduler)->ScheduleAll(tensor);
    }

private:
    void InitContext(const TCubeTiling* tiling) {
        var.tiling = tiling;
        var.singleCoreM_ = MM_CFG.singleCoreM;
        var.singleCoreN_ = MM_CFG.singleCoreN;
        var.singleCoreK_ = MM_CFG.singleCoreK;
        var.mIter_ = MM_CFG.singleCoreM / MM_CFG.basicM;
        var.nIter_ = MM_CFG.singleCoreN / MM_CFG.basicN;
        var.kIter_ = MM_CFG.singleCoreK / MM_CFG.basicK;
    }    

private:
    MatmulParams var;
};

}

#endif
