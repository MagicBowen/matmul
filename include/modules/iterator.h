/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef ITERATOR_H
#define ITERATOR_H

#include "matmul_keyword.h"
#include "matmul_config.h"

namespace matmul {

template <typename IMPL, const auto& MM_CFG>
class Iterator {
public:
    void Init(IterateOrder order, uint32_t mIter, uint32_t nIter, uint32_t kIter) {
        order_ = order;
        mIter_ = mIter;
        nIter_ = nIter;
        kIter_ = kIter;
        Reset();
    }

    template<typename EXECUTE>
    void Forward(size_t step, EXECUTE execute) {
        for (uint32_t i = 0; i < step; ++i) {
            execute(m_, n_, k_);
            if (!MoveNext()) break;
        }
    }

    void Reset() {
        m_ = 0;
        n_ = 0;
        k_ = 0;
    }

    bool End() const {
        return m_ >= mIter_ && n_ >= nIter_;
    }

private:
    bool MoveNext() {
        if (++k_ >= kIter_) {
            k_ = 0;
            if (++n_ >= nIter_) {
                n_ = 0;
                if (++m_ >= mIter_) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    IterateOrder order_;

    uint32_t mIter_;
    uint32_t nIter_;
    uint32_t kIter_;

    uint32_t m_;
    uint32_t n_;
    uint32_t k_;
};

} // namespace matmul

#endif
