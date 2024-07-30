/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "matmul_keyword.h"
#include "matmul_tiling.h"
#include "modules/iterate_controller.h"
#include "modules/copy_cube_in.h"
#include "modules/mmad.h"
#include "modules/co1_buffer.h"
#include "modules/copy_cube_out.h"

namespace matmul {

template<typename IMPL, typename A_TYPE, typename B_TYPE, typename C_TYPE, const auto& MM_CFG>
class Scheduler {
    MATMUL_USE_MODULE(IterateController);
    MATMUL_USE_MODULE(CopyCubeInA);
    MATMUL_USE_MODULE(CopyCubeInB);
    MATMUL_USE_MODULE(MMad);
    MATMUL_USE_MODULE(Co1Buffer);
    MATMUL_USE_MODULE(CopyCubeOut); 

public:
    void Init(const TCubeTiling* tiling) {
        MATMUL_MODULE(CopyCubeInA)->Init();
        MATMUL_MODULE(CopyCubeInB)->Init();
        MATMUL_MODULE(Co1Buffer)->Init();
    }

    void Destroy() {
        MATMUL_MODULE(CopyCubeInA)->Destroy();
        MATMUL_MODULE(CopyCubeInB)->Destroy();
        MATMUL_MODULE(Co1Buffer)->Destroy();
    }

    bool ScheduleOnce() {
        if (MATMUL_MODULE(IterateController)->MoveNext()) {
            return false;
        }

        auto row = MATMUL_MODULE(IterateController)->GetRowIndex();
        auto col = MATMUL_MODULE(IterateController)->GetColIndex();

        auto a = MATMUL_MODULE(CopyCubeInA)->Load(row, col);
        auto b = MATMUL_MODULE(CopyCubeInB)->Load(row, col);

        LocalTensor<typename C_TYPE::T> c = MATMUL_MODULE(Co1Buffer)->Alloc();
        MATMUL_MODULE(MMad)->Compute(c, a, b);

        MATMUL_MODULE(CopyCubeInA)->Clear(a);
        MATMUL_MODULE(CopyCubeInB)->Clear(b);

        return true;
    }

    template <typename TENSOR>
    void Schedule(TENSOR& tensor) {
        while (ScheduleOnce()) {
            auto srcTensor = MATMUL_MODULE(Co1Buffer)->Get();
            MATMUL_MODULE(CopyCubeOut)->Copy(tensor, srcTensor);
            MATMUL_MODULE(Co1Buffer)->Free(srcTensor);
        }
    }
};

}

#endif
