/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef ITERATE_CONTROLLER_H
#define ITERATE_CONTROLLER_H

#include "matmul_keyword.h"
#include "matmul_config.h"

namespace matmul {
//////////////////////////////////////////////////////////////////////
template<const auto& MM_CFG>
class IterateState {
public:
    void Init() {
        Reset();
    }

    uint32_t GetRowIndex() const {
        return curM_;
    }

    uint32_t GetColIndex() const {
        return curN_;
    }

    void Reset() {
        curM_ = 0;
        curN_ = 0;
        stepMIdx_ = 0;
        stepNIdx_ = 0;
        curStepM_ = MM_CFG.stepM;
        curStepN_ = MM_CFG.stepN;
    }

    bool IsFirstIterate() const {
        return curM_ == 0 && curN_ == 0;
    }

protected:
    uint32_t curM_{0};
    uint32_t curN_{0};
    uint32_t stepMIdx_{0};
    uint32_t stepNIdx_{0};
    uint32_t curStepM_{MM_CFG.stepM};
    uint32_t curStepN_{MM_CFG.stepN};
};

//////////////////////////////////////////////////////////////////////
template<const auto& MM_CFG, typename = void>
class IterateImpl : public IterateState<MM_CFG> {
    using State = IterateState<MM_CFG>;
public:
    auto& MainIdx() {
        return State::curM_;
    }

    auto& SubIdx() {
        return State::curN_;
    }

    auto& MainStepIdx() {
        return State::stepMIdx_;
    }

    auto& SubStepIdx() {
        return State::stepNIdx_;
    }

    auto& MainStep() {
        return State::curStepM_;
    }

    auto& SubStep() {
        return State::curStepN_;
    }

    template<typename CONTEXT>
    auto& MainIter(CONTEXT& ctx) {
        return ctx.mIter_;
    }

    template<typename CONTEXT>
    auto& SubIter(CONTEXT& ctx) {
        return ctx.nIter_;
    }
};

template<const auto& MM_CFG>
class IterateImpl<MM_CFG, std::enable_if_t<MM_CFG.iterOrder == IterateOrder::ORDER_N>> 
: public IterateState<MM_CFG> {
    using State = IterateState<MM_CFG>;
public:
    auto& MainIdx() {
        return State::curN_;
    }

    auto& SubIdx() {
        return State::curM_;
    }

    auto& MainStepIdx() {
        return State::stepNIdx_;
    }

    auto& SubStepIdx() {
        return State::stepMIdx_;
    }

    auto& MainStep() {
        return State::curStepN_;
    }

    auto& SubStep() {
        return State::curStepM_;
    }

    template<typename CONTEXT>
    auto& MainIter(CONTEXT& ctx) {
        return ctx.nIter_;
    }

    template<typename CONTEXT>
    auto& SubIter(CONTEXT& ctx) {
        return ctx.mIter_;
    }
};

//////////////////////////////////////////////////////////////////////
template <typename IMPL, const auto& MM_CFG>
class IterateController : private IterateImpl<MM_CFG> {

    using State = IterateState<MM_CFG>;
    using Iter  = IterateImpl<MM_CFG>;

public:
    using State::Init;
    using State::GetRowIndex;
    using State::GetColIndex;
    using State::Reset;

    bool MoveNext() {
        if (State::IsFirstIterate()) return true;

        if (++Iter::SubIdx() >= Iter::SubStepIdx() + Iter::SubStep()) {
            Iter::SubIdx() = Iter::SubStepIdx();
            if (++Iter::MainIdx() >= Iter::MainIter(MATMUL_CONTEXT())) {
                Iter::MainIdx() = 0;
                Iter::SubStepIdx() += Iter::SubStep();
                if (Iter::SubStepIdx() >= Iter::SubIter(MATMUL_CONTEXT())) {
                    return false;
                }
                Iter::SubIdx() = Iter::SubStepIdx();
            }
        }
        return true;
    }

    template <typename COMPUTE>
    void Reduce(COMPUTE compute) {
        for (uint32_t k = 0; k < MATMUL_CONTEXT().kIter_; ++k) {
            compute(State::GetRowIndex(), State::GetColIndex(), k);
        }
    }
};

}

#endif
