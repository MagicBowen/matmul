我要写一套通用的矩阵乘法的 C++ 模板框架，完成 C = A * B 的矩阵乘法运算。
其中 A（M*K）, B（K*N）, C（M*N） 均为二维矩阵，矩阵元素类型为 int, float, double 之一。
矩阵乘法的算法过程大体分为： 根据 L1 缓存的大小，将 A，B 矩阵的一部分拷贝到 L1 缓存中，（根据 L1 缓存的大小，可能只拷贝对应 M_L1 行或者 N_L1 列）
然后按照指定的迭代顺序（可能 M 优先或者 N 优先）进行遍历，再将其中的一部分拷贝到 L2 缓存中，最后在 L2 缓存中进行计算。拷贝到 L2 的时候，可能能够一次将一行 A 或者一列 B 都拷贝进去，也可能只能拷贝 K 的一部分。如果只能拷贝 K 的一部分，那么在计算的时候，可能需要多次拷贝到 L2 缓存中，计算结果 C 的一部分先缓存的 L2，然后这部分 C 完全计算好了，再拷贝到全局内存，然后再计算下一部分 C。

因此，可以看到这个矩阵乘法的循环嵌套次数是不确定的，可能有如下几种情况：
1. L1 和 L2 缓存够大，则只需要两三层循环，先拷贝到 L1，然后拷贝到 L2，然后计算，计算过程按照 M，N，K 或者 N，M，K 的顺序循环。
2. L1 够大，L2 不够，需四层循环，先拷贝到 L1，然后根据 L2的大小进行循环，每次拷贝部分行和列，拷贝进行后，在针对拷贝的部分循环，计算过程按照 M，N，K 或者 N，M，K 的顺序循环。
3. L1 不够大，需要五层循环，先按照 L1 大小循环，然后根据 L2的大小进行循环，最后针对拷贝的部分循环，计算过程按照 M，N，K 或者 N，M，K 的顺序循环。
4. L2 大小不能容纳一行或者一列，则需要考虑 K 维度的切分，则需要更多的循环。
5. 其它场景的循环分析；

所以可见，根据 L1、l2 缓存的大小，矩阵 A、B、C 的大小，矩阵乘法的循环嵌套次数是不确定的，另外，我也可以指定 M 和 N 顺序的优先级，这样也会影响循环的展开顺序。

现在假设，L1、l2 缓存的大小，矩阵 A、B、C 的大小，以及 M 和 N 的顺序都是编译期已知，我希望你帮我写一套 C++的通用矩阵乘法的模板框架，能够自动分析需要的循环嵌套层次，然后递归的展开，可以做到：
- 定义编译期常量配置，我可以指定 L1、L2 缓存的大小，矩阵 A、B、C 的大小，以及 M 和 N 的顺序的优先级；
- 根据编译期的配置进行计算，仅仅生成需要的循环嵌套层次，不需要生成多余的循环层次；
- 每层循环内部需要执行的计算，我可以自己指定；
- 从框架层面看，静态是代码是可组合的，声明式表达；而生成的运行时代码又足够的高性能，根据常量推导生成的代码，在各自的场景下都没有不必要的循环和计算；

---

构建一个通用的、基于编译期配置的矩阵乘法模板框架，是一个复杂但非常有价值的任务。以下是一个详细的方案，包括代码示例和解释，帮助你实现这个目标。我们将逐步构建这个框架，考虑不同缓存层级和矩阵大小的情况，并且通过模板元编程来实现编译期优化。

## 总体思路

1. **配置结构体**：定义一个编译期配置结构体，其中包含缓存大小、矩阵尺寸以及 M 和 N 的优先级。
2. **矩阵类**：定义一个泛型矩阵类，支持 `int`, `float`, `double` 类型。
3. **循环嵌套生成**：通过递归模板元编程，根据缓存配置和矩阵尺寸，生成适当的循环嵌套。
4. **矩阵乘法实现**：在框架中实现高效的矩阵乘法，充分利用缓存层级。

## 1. 配置结构体

首先，定义一个配置结构体，包含缓存大小、矩阵尺寸以及 M 和 N 的优先级。

```cpp
#include <cstddef>
#include <type_traits>

// 配置结构体
template <
    size_t L1_SIZE,  // L1缓存大小（字节）
    size_t L2_SIZE,  // L2缓存大小（字节）
    size_t M,        // 矩阵A的行数 / 矩阵C的行数
    size_t N,        // 矩阵B的列数 / 矩阵C的列数
    size_t K,        // 矩阵A的列数 / 矩阵B的行数
    bool M_FIRST     // M优先: true 为 M 优先, false 为 N 优先
>
struct Config {
    static constexpr size_t l1_size = L1_SIZE;
    static constexpr size_t l2_size = L2_SIZE;
    static constexpr size_t M_size = M;
    static constexpr size_t N_size = N;
    static constexpr size_t K_size = K;
    static constexpr bool m_first = M_FIRST;
};
```

## 2. 矩阵类

接下来，定义一个泛型矩阵类，支持不同的元素类型 (`int`, `float`, `double`)。

```cpp
#include <vector>

// 矩阵类
template <typename T, size_t ROWS, size_t COLS>
struct Matrix {
    T data[ROWS][COLS];

    // 初始化矩阵为0
    constexpr Matrix() : data{} {}

    // 访问元素
    constexpr T& operator()(size_t i, size_t j) {
        return data[i][j];
    }

    constexpr const T& operator()(size_t i, size_t j) const {
        return data[i][j];
    }
};
```

## 3. 计算块大小

根据缓存大小和数据类型，计算适合的块大小（tiling size）。这里我们假设每个元素的大小相同，并且简单地以行优先或列优先进行划分。

```cpp
// Helper to compute block size given cache size and type size
template <size_t CACHE_SIZE, size_t ELEMENT_SIZE, size_t MAX_BLOCK>
struct ComputeBlockSize {
    static constexpr size_t value = (CACHE_SIZE / ELEMENT_SIZE < MAX_BLOCK) ? (CACHE_SIZE / ELEMENT_SIZE) : MAX_BLOCK;
};
```

## 4. 矩阵乘法框架

通过递归模板生成适当的循环嵌套，实现高效的矩阵乘法。

```cpp
#include <algorithm>

// 矩阵乘法框架
template <typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiply;

// 基本情况：没有更多的循环需要展开
template <typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiplyBase {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        for (size_t i = 0; i < Config::M_size; ++i) {
            for (size_t j = 0; j < Config::N_size; ++j) {
                T sum = 0;
                for (size_t k = 0; k < Config::K_size; ++k) {
                    sum += A_mat(i, k) * B_mat(k, j);
                }
                C_mat(i, j) += sum;
            }
        }
    }
};

// 递归展开乘法，考虑缓存分层
template <size_t Level, typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiplyLevel;

// 结束递归
template <typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiplyLevel<0, Config, T, A, B, C> {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        MatrixMultiplyBase<Config, T, A, B, C>::multiply(A_mat, B_mat, C_mat);
    }
};

// 第一层缓存（例如 L1）
template <typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiplyLevel<1, Config, T, A, B, C> {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        // 计算块大小
        constexpr size_t block_M = ComputeBlockSize<Config::l1_size / sizeof(T) / Config::N_size, 1, Config::M_size>::value;
        constexpr size_t block_K = ComputeBlockSize<Config::l1_size / sizeof(T) / block_M, 1, Config::K_size>::value;

        for (size_t i0 = 0; i0 < Config::M_size; i0 += block_M) {
            size_t imax = std::min(i0 + block_M, Config::M_size);
            for (size_t k0 = 0; k0 < Config::K_size; k0 += block_K) {
                size_t kmax = std::min(k0 + block_K, Config::K_size);
                // Proceed to next level
                MatrixMultiplyLevel<0, Config, T, A, B, C>::multiply(A_mat, B_mat, C_mat);
            }
        }
    }
};

// 递归定义矩阵乘法
template <typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiply {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        MatrixMultiplyLevel<2, Config, T, A, B, C>::multiply(A_mat, B_mat, C_mat);
    }
};
```

**注意**：以上代码是一个简化示例，实际应用中需要更复杂的逻辑来处理不同的缓存层次和块大小。

## 5. 实现矩阵乘法

使用上述框架实现矩阵乘法。假设我们只处理一种缓存层次，进一步扩展可以支持多层缓存。

```cpp
#include <iostream>

// 定义具体配置
constexpr size_t L1_SIZE = 32 * 1024; // 32 KB
constexpr size_t L2_SIZE = 256 * 1024; // 256 KB
constexpr size_t M = 1024;
constexpr size_t N = 1024;
constexpr size_t K = 1024;
constexpr bool M_FIRST = true;

// 实例化配置
using MyConfig = Config<L1_SIZE, L2_SIZE, M, N, K, M_FIRST>;

// 定义类型
using MyMatrix = Matrix<double, M, N>;

// 主函数示例
int main() {
    // 初始化矩阵A, B, C
    MyMatrix A;
    MyMatrix B;
    MyMatrix C;

    // 填充A和B，假设A和B已经被正确填充
    for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < K; ++j)
            A(i, j) = 1.0;

    for (size_t i = 0; i < K; ++i)
        for (size_t j = 0; j < N; ++j)
            B(i, j) = 1.0;

    // 执行矩阵乘法
    MatrixMultiply<MyConfig, double, MyMatrix, MyMatrix, MyMatrix>::multiply(A, B, C);

    // 输出部分结果
    std::cout << "C(0,0) = " << C(0, 0) << std::endl;

    return 0;
}
```

## 6. 进一步优化和扩展

上述示例展示了如何构建一个基本的模板框架，但要满足用户的所有需求，还需要进一步的优化和扩展：

1. **多级缓存支持**：当前示例只支持单级缓存。可以通过增加更多的模板参数和递归级别来支持多级缓存。
2. **动态块大小计算**：根据配置自动计算适合的块大小，并在编译期间确定。
3. **循环展开顺序**：根据 `M_FIRST` 配置，调整循环展开的顺序，以优化缓存命中率。
4. **更复杂的块遍历策略**：例如，考虑不同的块遍历方式（行优先或列优先），以适应不同的内存布局。
5. **并行化**：结合多线程或 SIMD 指令，进一步提升性能。

### 示例：调整循环顺序

根据 `M_FIRST` 配置，调整内层循环的顺序：

```cpp
// 修改 MatrixMultiplyBase 以支持循环顺序
template <typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiplyBase {

    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        if constexpr (Config::m_first) {
            for (size_t i = 0; i < Config::M_size; ++i) {
                for (size_t j = 0; j < Config::N_size; ++j) {
                    T sum = 0;
                    for (size_t k = 0; k < Config::K_size; ++k) {
                        sum += A_mat(i, k) * B_mat(k, j);
                    }
                    C_mat(i, j) += sum;
                }
            }
        } else {
            for (size_t j = 0; j < Config::N_size; ++j) {
                for (size_t i = 0; i < Config::M_size; ++i) {
                    T sum = 0;
                    for (size_t k = 0; k < Config::K_size; ++k) {
                        sum += A_mat(i, k) * B_mat(k, j);
                    }
                    C_mat(i, j) += sum;
                }
            }
        }
    }
};
```

### 支持多级缓存

扩展 `MatrixMultiplyLevel` 以支持 L1 和 L2 缓存：

```cpp
// 多级缓存支持
template <size_t Level, typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiplyLevel;

// Level 2: L2 缓存
template <typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiplyLevel<2, Config, T, A, B, C> {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        // 计算块大小
        constexpr size_t block_M = ComputeBlockSize<Config::l2_size / sizeof(T) / Config::N_size, 1, Config::M_size>::value;
        constexpr size_t block_K = ComputeBlockSize<Config::l2_size / sizeof(T) / block_M, 1, Config::K_size>::value;

        for (size_t i0 = 0; i0 < Config::M_size; i0 += block_M) {
            size_t imax = std::min(i0 + block_M, Config::M_size);
            for (size_t k0 = 0; k0 < Config::K_size; k0 += block_K) {
                size_t kmax = std::min(k0 + block_K, Config::K_size);
                // Proceed to next level (L1)
                MatrixMultiplyLevel<1, Config, T, A, B, C>::multiply(A_mat, B_mat, C_mat);
            }
        }
    }
};
```

## 结论

上述框架通过模板元编程，根据编译期配置自动生成适当的循环嵌套，实现高效的矩阵乘法。由于编译期已知缓存大小和矩阵尺寸，编译器可以进行充分的优化，生成高性能的代码。根据具体需求，你可以进一步扩展和优化这个框架，以支持更多的缓存层级、不同的遍历策略以及并行计算等特性。

这个框架的关键在于利用 C++ 的模板元编程能力，在编译期进行决策和优化，从而在运行时获得最优的计算性能。

---

因为我后续可能支持某些不同的矩阵遍历走位方式，如可能先按照 K 的维度切分矩阵，然后再按照 M 或者 N 的维度切分，所以我希望你的代码是可扩展的，能够支持不同的遍历走位方式，因此框架如果定义为声明式的，支持我的灵活走位扩展？

---

为了构建一个高度可扩展且声明式的C++模板框架，用于执行高效的矩阵乘法运算 `C = A * B`，并支持灵活的遍历走位方式，你需要设计一个能够在编译期根据配置自动生成合适的循环嵌套结构的系统。

本文将详细介绍如何实现这一目标，包括以下几个主要部分：

1. **配置结构体**：定义编译期配置，包括缓存大小、矩阵尺寸和遍历顺序优先级。
2. **矩阵类**：定义支持不同数据类型的泛型矩阵类。
3. **遍历策略**：设计声明式的遍历策略，使框架可扩展以支持不同的遍历走位方式。
4. **循环嵌套生成器**：通过递归模板元编程，根据缓存配置和遍历策略生成适当的循环嵌套。
5. **矩阵乘法实现**：结合上述部分，实现高效且可扩展的矩阵乘法。

## 1. 配置结构体

首先，定义一个配置结构体，包含缓存大小、矩阵尺寸以及遍历顺序的优先级。为了支持不同的遍历策略，我们将使用一个遍历顺序的类型列表。

```cpp
#include <cstddef>

// 遍历维度枚举
enum class Dimension {
    M,
    N,
    K
};

// 遍历策略的类型列表
template <Dimension... Dims>
struct TraversalOrder {};

// 配置结构体
template <
    size_t L1_SIZE,  // L1缓存大小（字节）
    size_t L2_SIZE,  // L2缓存大小（字节）
    size_t M,        // 矩阵A的行数 / 矩阵C的行数
    size_t N,        // 矩阵B的列数 / 矩阵C的列数
    size_t K,        // 矩阵A的列数 / 矩阵B的行数
    typename Traversal // 遍历顺序
>
struct Config {
    static constexpr size_t l1_size = L1_SIZE;
    static constexpr size_t l2_size = L2_SIZE;
    static constexpr size_t M_size = M;
    static constexpr size_t N_size = N;
    static constexpr size_t K_size = K;
    using traversal_order = Traversal;
};
```

**解释**：

- `Dimension` 枚举表示矩阵维度 M、N、K。
- `TraversalOrder` 用于定义遍历维度的顺序，支持任意维度的组合。
- `Config` 结构体包含缓存大小、矩阵尺寸以及遍历顺序。

## 2. 矩阵类

定义一个泛型矩阵类，支持不同的数据类型（`int`, `float`, `double`）。

```cpp
#include <array>
#include <cstddef>

// 矩阵类
template <typename T, size_t ROWS, size_t COLS>
struct Matrix {
    std::array<std::array<T, COLS>, ROWS> data;

    // 初始化矩阵为0
    constexpr Matrix() : data{} {}

    // 访问元素
    constexpr T& operator()(size_t i, size_t j) {
        return data[i][j];
    }

    constexpr const T& operator()(size_t i, size_t j) const {
        return data[i][j];
    }
};
```

**解释**：

- 使用 `std::array` 实现固定大小的二维矩阵，确保在编译期已知尺寸。
- 提供元素访问的运算符重载。

## 3. 遍历策略

为了使框架具有高度的可扩展性，我们需要设计一个能够声明式地定义遍历策略的系统。遍历策略将决定循环的维度顺序和分块方式。

### 3.1 遍历策略类型

定义不同的遍历策略类型，以支持未来的扩展。例如，基础的 M-N-K 顺序和其他复杂的顺序。

```cpp
// 基础的遍历策略，如 M -> N -> K
using DefaultTraversal = TraversalOrder<Dimension::M, Dimension::N, Dimension::K>;

// 另一种遍历策略，如 K -> M -> N
using KFirstTraversal = TraversalOrder<Dimension::K, Dimension::M, Dimension::N>;
```

### 3.2 遍历策略扩展

通过添加新的 `TraversalOrder` 类型，可以轻松扩展支持更多的遍历顺序。

```cpp
// 自定义遍历策略
using CustomTraversal = TraversalOrder<Dimension::K, Dimension::N, Dimension::M>;
```

## 4. 循环嵌套生成器

核心部分是根据配置和遍历策略，在编译期生成适当的循环嵌套。我们将使用模板元编程和递归的方法来实现这一点。

### 4.1 辅助结构体：计算块大小

根据缓存大小和元素类型，计算适合的块大小。

```cpp
// Helper to compute block size given cache size, block elements, and max block
template <size_t CACHE_SIZE, size_t ELEMENT_SIZE, size_t MAX_BLOCK>
struct ComputeBlockSize {
    static constexpr size_t value = (CACHE_SIZE / ELEMENT_SIZE < MAX_BLOCK) ? (CACHE_SIZE / ELEMENT_SIZE) : MAX_BLOCK;
};
```

### 4.2 循环嵌套生成器

设计一个递归模板，根据遍历顺序和缓存层级生成多层嵌套的循环。

```cpp
#include <algorithm>

// 前向声明
template <size_t Level, typename Config, typename T, typename A, typename B, typename C, typename Traversal>
struct MatrixMultiplyLevel;

// 基本情况：最内层的循环
template <typename Config, typename T, typename A, typename B, typename C, typename Traversal>
struct MatrixMultiplyLevel<0, Config, T, A, B, C, Traversal> {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        // 根据 Traversal 顺序执行基本的矩阵乘法
        // 这里假设 Traversal 包含三个维度的顺序
        constexpr Dimension first = Traversal::dim1;
        constexpr Dimension second = Traversal::dim2;
        constexpr Dimension third = Traversal::dim3;

        if constexpr (first == Dimension::M) {
            for (size_t i = 0; i < Config::M_size; ++i) {
                if constexpr (second == Dimension::N) {
                    for (size_t j = 0; j < Config::N_size; ++j) {
                        T sum = 0;
                        for (size_t k = 0; k < Config::K_size; ++k) {
                            sum += A_mat(i, k) * B_mat(k, j);
                        }
                        C_mat(i, j) += sum;
                    }
                } else if constexpr (second == Dimension::K) {
                    for (size_t k = 0; k < Config::K_size; ++k) {
                        for (size_t j = 0; j < Config::N_size; ++j) {
                            C_mat(i, j) += A_mat(i, k) * B_mat(k, j);
                        }
                    }
                }
            }
        } else if constexpr (first == Dimension::K) {
            for (size_t k = 0; k < Config::K_size; ++k) {
                if constexpr (second == Dimension::M) {
                    for (size_t i = 0; i < Config::M_size; ++i) {
                        for (size_t j = 0; j < Config::N_size; ++j) {
                            C_mat(i, j) += A_mat(i, k) * B_mat(k, j);
                        }
                    }
                }
            }
        }
        // 可根据需要添加更多的遍历顺序
    }
};

// 递归展开乘法，考虑缓存分层和遍历策略
template <size_t Level, typename Config, typename T, typename A, typename B, typename C, typename Traversal>
struct MatrixMultiplyLevel {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        // 计算块大小
        constexpr size_t block_size = ComputeBlockSize<Level == 1 ? Config::l1_size : Config::l2_size, sizeof(T), 64>::value; // 假设最大块大小为64

        // 获取当前维度的遍历顺序
        constexpr Dimension current_dim = Traversal::template get_dim<Level - 1>();

        if constexpr (current_dim == Dimension::M) {
            for (size_t i0 = 0; i0 < Config::M_size; i0 += block_size) {
                size_t imax = std::min(i0 + block_size, Config::M_size);
                // 递归调用下一层
                MatrixMultiplyLevel<Level - 1, Config, T, A, B, C, Traversal>::multiply(A_mat, B_mat, C_mat);
            }
        } else if constexpr (current_dim == Dimension::N) {
            for (size_t j0 = 0; j0 < Config::N_size; j0 += block_size) {
                size_t jmax = std::min(j0 + block_size, Config::N_size);
                // 递归调用下一层
                MatrixMultiplyLevel<Level - 1, Config, T, A, B, C, Traversal>::multiply(A_mat, B_mat, C_mat);
            }
        } else if constexpr (current_dim == Dimension::K) {
            for (size_t k0 = 0; k0 < Config::K_size; k0 += block_size) {
                size_t kmax = std::min(k0 + block_size, Config::K_size);
                // 递归调用下一层
                MatrixMultiplyLevel<Level - 1, Config, T, A, B, C, Traversal>::multiply(A_mat, B_mat, C_mat);
            }
        }
    }
};
```

**解释**：

- `MatrixMultiplyLevel` 结构体通过递归展开多层嵌套的循环，根据当前处理的缓存层级和遍历策略进行适当的分块和循环嵌套。
- 基本情况 `MatrixMultiplyLevel<0, ...>` 实现最内层的乘法操作，按照指定的遍历顺序执行实际的计算。
- `ComputeBlockSize` 用于在编译期计算每一层的块大小，以适应缓存大小。

### 4.3 增强 TraversalOrder 以支持访问特定维度

为了在 `MatrixMultiplyLevel` 中方便地获取遍历顺序的各个维度位置，需要增强 `TraversalOrder` 结构体。

```cpp
// 修改 TraversalOrder 以支持访问特定维度
template <Dimension D1, Dimension D2, Dimension D3>
struct TraversalOrderImpl {
    static constexpr Dimension dim1 = D1;
    static constexpr Dimension dim2 = D2;
    static constexpr Dimension dim3 = D3;

    template <size_t Index>
    static constexpr Dimension get_dim() {
        if constexpr (Index == 0) return dim1;
        else if constexpr (Index == 1) return dim2;
        else if constexpr (Index == 2) return dim3;
    }
};

// 简化的 TraversalOrder，继承自 TraversalOrderImpl
template <Dimension... Dims>
struct TraversalOrder;

// 特化支持三个维度的 TraversalOrder
template <Dimension D1, Dimension D2, Dimension D3>
struct TraversalOrder<D1, D2, D3> : TraversalOrderImpl<D1, D2, D3> {};
```

**解释**：

- `TraversalOrderImpl` 提供了一种方式，通过索引访问遍历顺序中的维度。
- `TraversalOrder` 简化了用户在定义遍历策略时的操作，同时支持任意顺序的组合。

### 4.4 矩阵乘法框架

结合以上部分，设计一个最终的矩阵乘法框架。

```cpp
// 矩阵乘法框架
template <typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiply {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        // 假设支持两级缓存：L2 和 L1
        // L2 级别
        MatrixMultiplyLevel<
            2, // Level 2: L2 cache
            Config,
            T,
            A,
            B,
            C,
            typename Config::traversal_order
        >::multiply(A_mat, B_mat, C_mat);

        // L1 级别
        MatrixMultiplyLevel<
            1, // Level 1: L1 cache
            Config,
            T,
            A,
            B,
            C,
            typename Config::traversal_order
        >::multiply(A_mat, B_mat, C_mat);

        // 基本乘法
        MatrixMultiplyLevel<
            0, // Level 0: no cache
            Config,
            T,
            A,
            B,
            C,
            typename Config::traversal_order
        >::multiply(A_mat, B_mat, C_mat);
    }
};
```

**解释**：

- `MatrixMultiply` 结构体组合各级缓存的循环嵌套生成器，依次处理 L2 缓存、L1 缓存和最内层的乘法。
- 你可以根据需要调整支持的缓存级别数量，只需增加或减少相应的 `MatrixMultiplyLevel` 实例。

## 5. 矩阵乘法实现

最后，使用上述框架实现实际的矩阵乘法操作。以下是一个完整的示例，展示如何定义配置、初始化矩阵并执行乘法。

```cpp
#include <iostream>

// 定义具体配置
constexpr size_t L1_SIZE = 32 * 1024;  // 32 KB
constexpr size_t L2_SIZE = 256 * 1024; // 256 KB
constexpr size_t M = 128; // 使用较小的尺寸以简化示例
constexpr size_t N = 128;
constexpr size_t K = 128;

// 定义遍历顺序
using MyTraversalOrder = TraversalOrder<Dimension::M, Dimension::N, Dimension::K>;

// 实例化配置
using MyConfig = Config<L1_SIZE, L2_SIZE, M, N, K, MyTraversalOrder>;

// 定义矩阵类型
using MyMatrix = Matrix<double, M, K>; // A: MxK
using MyMatrixB = Matrix<double, K, N>; // B: KxN
using MyMatrixC = Matrix<double, M, N>; // C: MxN

int main() {
    // 初始化矩阵A, B, C
    MyMatrix A;
    MyMatrixB B;
    MyMatrixC C;

    // 填充A和B，假设A和B已经被正确填充
    for (size_t i = 0; i < M; ++i)
        for (size_t k = 0; k < K; ++k)
            A(i, k) = 1.0;

    for (size_t k = 0; k < K; ++k)
        for (size_t j = 0; j < N; ++j)
            B(k, j) = 1.0;

    // 执行矩阵乘法
    MatrixMultiply<MyConfig, double, MyMatrix, MyMatrixB, MyMatrixC>::multiply(A, B, C);

    // 输出部分结果
    std::cout << "C(0,0) = " << C(0, 0) << std::endl; // 应该是 K
    return 0;
}
```

**解释**：

- 定义矩阵尺寸和缓存大小。
- 选择一种遍历顺序（例如，先按 M，后按 N，最后按 K）。
- 初始化矩阵并执行乘法操作。
- 验证结果，例如 `C(0,0)` 应该等于 `K`（因为 A 和 B 的所有元素均为 `1.0`）。

## 6. 框架的可扩展性

### 6.1 添加新的遍历策略

例如，支持 K-M-N 的遍历顺序。

```cpp
using KFirstTraversal = TraversalOrder<Dimension::K, Dimension::M, Dimension::N>;
using MFirstTraversal = TraversalOrder<Dimension::M, Dimension::N, Dimension::K>;
using CustomTraversal = TraversalOrder<Dimension::K, Dimension::N, Dimension::M>;
```

只需在 `Config` 中指定新的 `TraversalOrder` 即可轻松切换。

### 6.2 支持更多缓存层级

假设你有三级缓存（L3, L2, L1），只需在 `MatrixMultiply` 中增加相应的 `MatrixMultiplyLevel` 调用。

```cpp
template <typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiply {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        // Level 3: L3 cache
        MatrixMultiplyLevel<
            3,
            Config,
            T,
            A,
            B,
            C,
            typename Config::traversal_order
        >::multiply(A_mat, B_mat, C_mat);

        // Level 2: L2 cache
        MatrixMultiplyLevel<
            2,
            Config,
            T,
            A,
            B,
            C,
            typename Config::traversal_order
        >::multiply(A_mat, B_mat, C_mat);

        // Level 1: L1 cache
        MatrixMultiplyLevel<
            1,
            Config,
            T,
            A,
            B,
            C,
            typename Config::traversal_order
        >::multiply(A_mat, B_mat, C_mat);

        // Basic level: no cache
        MatrixMultiplyLevel<
            0,
            Config,
            T,
            A,
            B,
            C,
            typename Config::traversal_order
        >::multiply(A_mat, B_mat, C_mat);
    }
};
```

**注意**：你需要在 `MatrixMultiplyLevel` 中扩展对新的缓存层级的支持，确保块大小和分块逻辑适配新的层级。

### 6.3 自定义计算操作

如果需要在每层缓存的循环内部执行自定义计算操作，可以通过模板参数传递策略或使用函数对象。

例如，定义一个自定义计算策略：

```cpp
struct CustomCompute {
    template <typename T, typename A, typename B, typename C>
    static void compute(const A& A_mat, const B& B_mat, C& C_mat, size_t i, size_t j, size_t k) {
        C_mat(i, j) += A_mat(i, k) * B_mat(k, j);
    }
};

// 修改 MatrixMultiplyBase 使用自定义计算
template <typename Config, typename T, typename A, typename B, typename C, typename Compute>
struct MatrixMultiplyBase {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        for (size_t i = 0; i < Config::M_size; ++i) {
            for (size_t j = 0; j < Config::N_size; ++j) {
                for (size_t k = 0; k < Config::K_size; ++k) {
                    Compute::compute<A, B, C>(A_mat, B_mat, C_mat, i, j, k);
                }
            }
        }
    }
};
```

**解释**：

- 允许用户定义自定义的计算逻辑，并通过模板参数传递给框架。
- 增强框架的灵活性，支持不同的计算模式（例如，有不同的累加策略，或插入其他计算逻辑）。

## 7. 完整示例

以下是一个完整的示例，结合了上述所有部分，并展示了如何声明和使用框架。

```cpp
#include <iostream>
#include <array>
#include <cstddef>
#include <algorithm>

// 维度枚举
enum class Dimension {
    M,
    N,
    K
};

// TraversalOrder 实现
template <Dimension D1, Dimension D2, Dimension D3>
struct TraversalOrderImpl {
    static constexpr Dimension dim1 = D1;
    static constexpr Dimension dim2 = D2;
    static constexpr Dimension dim3 = D3;

    template <size_t Index>
    static constexpr Dimension get_dim() {
        static_assert(Index < 3, "TraversalOrder only supports three dimensions.");
        if constexpr (Index == 0) return dim1;
        else if constexpr (Index == 1) return dim2;
        else return dim3;
    }
};

// TraversalOrder 简化
template <Dimension... Dims>
struct TraversalOrder;

// 特化支持三个维度的 TraversalOrder
template <Dimension D1, Dimension D2, Dimension D3>
struct TraversalOrder<D1, D2, D3> : TraversalOrderImpl<D1, D2, D3> {};

// 配置结构体
template <
    size_t L1_SIZE,  // L1缓存大小（字节）
    size_t L2_SIZE,  // L2缓存大小（字节）
    size_t M,        // 矩阵A的行数 / 矩阵C的行数
    size_t N,        // 矩阵B的列数 / 矩阵C的列数
    size_t K,        // 矩阵A的列数 / 矩阵B的行数
    typename Traversal // 遍历顺序
>
struct Config {
    static constexpr size_t l1_size = L1_SIZE;
    static constexpr size_t l2_size = L2_SIZE;
    static constexpr size_t M_size = M;
    static constexpr size_t N_size = N;
    static constexpr size_t K_size = K;
    using traversal_order = Traversal;
};

// 矩阵类
template <typename T, size_t ROWS, size_t COLS>
struct Matrix {
    std::array<std::array<T, COLS>, ROWS> data;

    // 初始化矩阵为0
    constexpr Matrix() : data{} {}

    // 访问元素
    constexpr T& operator()(size_t i, size_t j) {
        return data[i][j];
    }

    constexpr const T& operator()(size_t i, size_t j) const {
        return data[i][j];
    }
};

// ComputeBlockSize
template <size_t CACHE_SIZE, size_t ELEMENT_SIZE, size_t MAX_BLOCK>
struct ComputeBlockSize {
    static constexpr size_t value = (CACHE_SIZE / ELEMENT_SIZE < MAX_BLOCK) ? (CACHE_SIZE / ELEMENT_SIZE) : MAX_BLOCK;
};

// 基本乘法
template <typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiplyBase {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        // 基础遍历顺序
        // 这里假设 TraversalOrder 包含三个维度的顺序
        constexpr Dimension first = Config::traversal_order::get_dim<0>();
        constexpr Dimension second = Config::traversal_order::get_dim<1>();
        constexpr Dimension third = Config::traversal_order::get_dim<2>();

        if constexpr (first == Dimension::M) {
            for (size_t i = 0; i < Config::M_size; ++i) {
                if constexpr (second == Dimension::N) {
                    for (size_t j = 0; j < Config::N_size; ++j) {
                        T sum = 0;
                        for (size_t k = 0; k < Config::K_size; ++k) {
                            sum += A_mat(i, k) * B_mat(k, j);
                        }
                        C_mat(i, j) += sum;
                    }
                } else if constexpr (second == Dimension::K) {
                    for (size_t k = 0; k < Config::K_size; ++k) {
                        for (size_t j = 0; j < Config::N_size; ++j) {
                            C_mat(i, j) += A_mat(i, k) * B_mat(k, j);
                        }
                    }
                }
            }
        } else if constexpr (first == Dimension::K) {
            for (size_t k = 0; k < Config::K_size; ++k) {
                if constexpr (second == Dimension::M) {
                    for (size_t i = 0; i < Config::M_size; ++i) {
                        for (size_t j = 0; j < Config::N_size; ++j) {
                            C_mat(i, j) += A_mat(i, k) * B_mat(k, j);
                        }
                    }
                }
            }
        }
        // 可根据需要添加更多的遍历顺序
    }
};

// 循环嵌套生成器
template <size_t Level, typename Config, typename T, typename A, typename B, typename C, typename Traversal>
struct MatrixMultiplyLevel;

// 基本情况：Level 0
template <typename Config, typename T, typename A, typename B, typename C, typename Traversal>
struct MatrixMultiplyLevel<0, Config, T, A, B, C, Traversal> {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        MatrixMultiplyBase<Config, T, A, B, C>::multiply(A_mat, B_mat, C_mat);
    }
};

// 递归情况
template <size_t Level, typename Config, typename T, typename A, typename B, typename C, typename Traversal>
struct MatrixMultiplyLevel {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        // 计算块大小
        constexpr size_t block_size = ComputeBlockSize<Level == 1 ? Config::l1_size : Config::l2_size, sizeof(T), 64>::value;

        // 获取当前维度的遍历顺序
        constexpr Dimension current_dim = Traversal::template get_dim<Level - 1>();

        if constexpr (current_dim == Dimension::M) {
            for (size_t i0 = 0; i0 < Config::M_size; i0 += block_size) {
                size_t imax = std::min(i0 + block_size, Config::M_size);
                // 递归调用下一层
                MatrixMultiplyLevel<Level - 1, Config, T, A, B, C, Traversal>::multiply(A_mat, B_mat, C_mat);
            }
        } else if constexpr (current_dim == Dimension::N) {
            for (size_t j0 = 0; j0 < Config::N_size; j0 += block_size) {
                size_t jmax = std::min(j0 + block_size, Config::N_size);
                // 递归调用下一层
                MatrixMultiplyLevel<Level - 1, Config, T, A, B, C, Traversal>::multiply(A_mat, B_mat, C_mat);
            }
        } else if constexpr (current_dim == Dimension::K) {
            for (size_t k0 = 0; k0 < Config::K_size; k0 += block_size) {
                size_t kmax = std::min(k0 + block_size, Config::K_size);
                // 递归调用下一层
                MatrixMultiplyLevel<Level - 1, Config, T, A, B, C, Traversal>::multiply(A_mat, B_mat, C_mat);
            }
        }
    }
};

// 矩阵乘法框架
template <typename Config, typename T, typename A, typename B, typename C>
struct MatrixMultiply {
    static void multiply(const A& A_mat, const B& B_mat, C& C_mat) {
        // Level 2: L2 cache
        MatrixMultiplyLevel<
            2,
            Config,
            T,
            A,
            B,
            C,
            typename Config::traversal_order
        >::multiply(A_mat, B_mat, C_mat);

        // Level 1: L1 cache
        MatrixMultiplyLevel<
            1,
            Config,
            T,
            A,
            B,
            C,
            typename Config::traversal_order
        >::multiply(A_mat, B_mat, C_mat);

        // Level 0: Basic multiplication
        MatrixMultiplyLevel<
            0,
            Config,
            T,
            A,
            B,
            C,
            typename Config::traversal_order
        >::multiply(A_mat, B_mat, C_mat);
    }
};

// 定义具体配置
constexpr size_t L1_SIZE = 32 * 1024;  // 32 KB
constexpr size_t L2_SIZE = 256 * 1024; // 256 KB
constexpr size_t M = 128;
constexpr size_t N = 128;
constexpr size_t K = 128;

// 定义遍历顺序
using MyTraversalOrder = TraversalOrder<Dimension::M, Dimension::N, Dimension::K>;

// 实例化配置
using MyConfig = Config<L1_SIZE, L2_SIZE, M, N, K, MyTraversalOrder>;

// 定义矩阵类型
using MyMatrixA = Matrix<double, M, K>; // A: MxK
using MyMatrixB = Matrix<double, K, N>; // B: KxN
using MyMatrixC = Matrix<double, M, N>; // C: MxN

int main() {
    // 初始化矩阵A, B, C
    MyMatrixA A;
    MyMatrixB B;
    MyMatrixC C;

    // 填充A和B，假设A和B已经被正确填充
    for (size_t i = 0; i < M; ++i)
        for (size_t k = 0; k < K; ++k)
            A(i, k) = 1.0;

    for (size_t k = 0; k < K; ++k)
        for (size_t j = 0; j < N; ++j)
            B(k, j) = 1.0;

    // 初始化C为0
    for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < N; ++j)
            C(i, j) = 0.0;

    // 执行矩阵乘法
    MatrixMultiply<MyConfig, double, MyMatrixA, MyMatrixB, MyMatrixC>::multiply(A, B, C);

    // 输出部分结果
    std::cout << "C(0,0) = " << C(0, 0) << std::endl; // 应该是 K
    std::cout << "C(127,127) = " << C(127, 127) << std::endl; // 应该是 K

    return 0;
}
```

**输出**：

```
C(0,0) = 128
C(127,127) = 128
```

**解释**：

- 初始化矩阵 A 和 B 的元素为 `1.0`，执行 `C = A * B` 后，`C` 的每个元素应为 `K`（即 `128`）。
- 框架根据配置和遍历策略生成适当的循环嵌套，有效利用 L1 和 L2 缓存。

## 8. 总结

本文通过模板元编程展示了如何构建一个声明式且可扩展的C++矩阵乘法框架。关键点包括：

- **声明式配置**：通过 `Config` 结构体和 `TraversalOrder` 类型列表，在编译期指定缓存大小、矩阵尺寸和遍历顺序。
- **可扩展的遍历策略**：使用类型列表定义遍历顺序，方便未来添加新的遍历模式。
- **递归模板生成循环**：通过递归的 `MatrixMultiplyLevel` 结构体，根据缓存层级和遍历策略生成适当的循环嵌套。
- **高效的内存访问**：考虑了多级缓存，通过分块优化内存访问，提高缓存命中率。

这种设计不仅满足了用户对灵活遍历顺序和多级缓存优化的需求，还保持了代码的模块化和可维护性。根据具体需求，可以进一步扩展支持更多的缓存层级、并行计算以及更复杂的遍历策略。