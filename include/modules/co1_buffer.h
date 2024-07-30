/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef CO1_BUFFER_H
#define CO1_BUFFER_H

namespace matmul {

template<typename IMPL, typename TYPE, const auto& MM_CFG>
class Co1Buffer {
    using DstT = typename TYPE::T;
public:
    void Init() {
    }

    void Destroy() {
    }

    LocalTensor<DstT> Alloc() {
        LocalTensor<DstT> tensor;
        return tensor;
    }

    void Free(LocalTensor<DstT>& tensor) {
    }

    LocalTensor<DstT> Get() {
        LocalTensor<DstT> tensor;
        return tensor;
    }
};

}

#endif
