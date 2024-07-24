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

public:
    void Init(const TCubeTiling* tiling) {
        var.tiling = tiling;
        var.singleCoreM_ = tiling->singleCoreM;
        var.singleCoreN_ = tiling->singleCoreN;
        var.singleCoreK_ = tiling->singleCoreK;
    }

    void SetTensorA(const GlobalTensor<SrcAT>&);
    void SetTensorA(const LocalTensor<SrcAT>&);
    void SetTensorB(const GlobalTensor<SrcBT>&);
    void SetTensorB(const LocalTensor<SrcBT>&);
    void SetBias(const GlobalTensor<BiasT>&);
    void SetBias(const LocalTensor<BiasT>&);

    void GetTensorC(const LocalTensor<DstT>&);
    void GetTensorC(const GlobalTensor<DstT>&);

    bool Iterate() {
        return IterateController::IsFinished();
    }

    void IterateAll(const GlobalTensor<DstT>&);
    void IterateAll(const LocalTensor<DstT>&) {
        Scheduler::Schedule();
    }

private:
    MatmulParams var;
};

}

#endif
