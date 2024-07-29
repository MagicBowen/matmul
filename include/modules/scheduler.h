/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "matmul_keyword.h"
#include "modules/iterate_controller.h"
#include "modules/mmad.h"

namespace matmul {

template<typename IMPL, const auto& MM_CFG>
class Scheduler {
    MATMUL_USE_MODULE(IterateController);
    MATMUL_USE_MODULE(CopyCubeInA);
    MATMUL_USE_MODULE(CopyCubeInB);
    MATMUL_USE_MODULE(MMad);

public:
    template<typename C_TYPE>
    bool Schedule(LocalTensor<C_TYPE>& c) {
        if (MATMUL_MODULE(IterateController)->MoveNext()) {
            return false;
        }

        auto row = MATMUL_MODULE(IterateController)->GetRowIndex();
        auto col = MATMUL_MODULE(IterateController)->GetColIndex();

        auto a = MATMUL_MODULE(CopyCubeInA)->Load(row, col, 1, 1);
        auto b = MATMUL_MODULE(CopyCubeInB)->Load(row, col, 1, 1);

        MATMUL_MODULE(MMad)->Compute(c, a, b);

        MATMUL_MODULE(CopyCubeInA)->Clear(a);
        MATMUL_MODULE(CopyCubeInB)->Clear(b);

        return true;
    }
};

}

#endif
