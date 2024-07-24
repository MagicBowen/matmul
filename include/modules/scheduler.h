/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "matmul_keyword.h"
#include "modules/iterate_controller.h"

namespace matmul {

template<typename IMPL, const auto& MM_CFG>
class Scheduler {
    MATMUL_USE_MODULE(IterateController);

public:
    bool ScheduleOnce() {
        if (MATMUL_MODULE(IterateController).IsFinished()) {
            return false;
        }
        return true;
    }

    void Schedule() {
        do {

        } while(MATMUL_MODULE(IterateController).MoveNext());
    }
};

}

#endif
