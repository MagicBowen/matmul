/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_TENSOR_H
#define MATMUL_TENSOR_H

#include <stdint.h>
#include <iostream>

namespace  {

template<typename T>
struct LocalTensor {
    void SetAddr(T* addr, uint32_t length) {
        this->addr = addr;
        this->length = length;
    }

    T* GetAddr() {
        return this->addr;
    }

    const T* GetAddr() const {
        return this->addr;
    }

    uint32_t GetLength() const {
        return length;
    }

private:
    T* addr;
    uint32_t length;
};

template<typename T>
struct GlobalTensor {
    void SetAddr(T* addr, uint32_t length) {
        this->addr = addr;
        this->length = length;
    }

    T* GetAddr() {
        return this->addr;
    }

    const T* GetAddr() const {
        return this->addr;
    }

    uint32_t GetLength() const {
        return length;
    }

private:
    T* addr;
    uint32_t length;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const LocalTensor<T>& tensor) {
    os << "LocalTensor(" << tensor.GetAddr() << " : " << tensor.GetLength() << ")";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const GlobalTensor<T>& tensor) {
    os << "LocalTensor(" << tensor.GetAddr() << " : " << tensor.GetLength() << ")";
    return os;
}

}

#endif
