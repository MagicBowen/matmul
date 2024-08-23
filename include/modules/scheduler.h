/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "matmul_keyword.h"
#include "matmul_tiling.h"
#include "modules/iterate_controller.h"
#include "modules/copy_cube_in.h"
#include "modules/split_load.h"
#include "modules/mmad.h"
#include "modules/co1_buffer.h"
#include "modules/copy_cube_out.h"

namespace matmul {

template<typename IMPL, typename A_TYPE, typename B_TYPE, typename C_TYPE, const auto& MM_CFG>
class Scheduler {
    // MATMUL_USE_MODULE(IterateController);
    MATMUL_USE_MODULE(Iterator);
    MATMUL_USE_MODULE(CopyCubeInA);
    MATMUL_USE_MODULE(CopyCubeInB);
    MATMUL_USE_MODULE(SplitLoadA);
    MATMUL_USE_MODULE(SplitLoadB);
    MATMUL_USE_MODULE(MMad);
    MATMUL_USE_MODULE(Co1Buffer);
    MATMUL_USE_MODULE(CopyCubeOut); 

public:
    void Init(const TCubeTiling* tiling) {
        // MATMUL_MODULE(IterateController)->Init();
        MATMUL_MODULE(CopyCubeInA)->Init();
        MATMUL_MODULE(CopyCubeInB)->Init();
        MATMUL_MODULE(Co1Buffer)->Init();
    }

    void Destroy() {
        MATMUL_MODULE(CopyCubeInA)->Destroy();
        MATMUL_MODULE(CopyCubeInB)->Destroy();
        MATMUL_MODULE(Co1Buffer)->Destroy();
    }

    // bool ScheduleOnce() {
    //     if (!MATMUL_MODULE(IterateController)->Forward()) {
    //         return false;
    //     }

    //     MATMUL_MODULE(IterateController)->Reduce([this](auto m, auto n, auto k) {
    //         auto tensorA = MATMUL_MODULE(CopyCubeInA)->Load(m, k);
    //         auto tensorB = MATMUL_MODULE(CopyCubeInB)->Load(k, n);

    //         auto c = MATMUL_MODULE(Co1Buffer)->Alloc();
    //         auto a = MATMUL_MODULE(SplitLoadA)->Split(tensorA, k);
    //         auto b = MATMUL_MODULE(SplitLoadB)->Split(tensorB, k);

    //         MATMUL_MODULE(MMad)->Compute(c, a, b);

    //         MATMUL_MODULE(CopyCubeInA)->Clear(tensorA);
    //         MATMUL_MODULE(CopyCubeInB)->Clear(tensorB);
    //     });

    //     return true;
    // }

    bool Schedule() {
        if (MATMUL_MODULE(Iterator)->End()) {
            return false;
        }

        auto tensorA = MATMUL_MODULE(CopyCubeInA)->Load();
        auto tensorB = MATMUL_MODULE(CopyCubeInB)->Load();
        
        auto step = GetStep(tensorA, tensorB);

        MATMUL_MODULE(Iterator)->Forward(step, [&, this](auto m, auto n, auto k) {
            auto c = MATMUL_MODULE(Co1Buffer)->Alloc();
            auto a = MATMUL_MODULE(SplitLoadA)->Load(tensorA, k);
            auto b = MATMUL_MODULE(SplitLoadB)->Load(tensorB, k);

            MATMUL_MODULE(MMad)->Compute(c, a, b);            
        });

        MATMUL_MODULE(CopyCubeInA)->Clear(tensorA);
        MATMUL_MODULE(CopyCubeInB)->Clear(tensorB);
        return true;
    }

    template <typename TENSOR>
    void ScheduleAll(TENSOR& tensor) {
        while (Schedule()) {
            auto l0cTensor = MATMUL_MODULE(Co1Buffer)->Get();
            MATMUL_MODULE(CopyCubeOut)->Copy(tensor, l0cTensor);
            MATMUL_MODULE(Co1Buffer)->Free(l0cTensor);
        }
    }

private:
    template<typename TENSOR>
    size_t GetStep(const TENSOR& tensorA, const TENSOR& tensorB) {
        return (tensorA.GetLength() < tensorB.GetLength()) ? tensorA.GetLength() : tensorB.GetLength();
    }
};

}

#endif
