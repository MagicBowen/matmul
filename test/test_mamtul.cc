#include "catch2/catch.hpp"
#include "matmul_impl.h"

using namespace matmul;

namespace {
    template<typename T1, typename T2, typename T3>
    void matrix_multiply(const T1& a, const T2& b, T3& c) {
        constexpr auto M = std::extent<T1, 0>::value;
        constexpr auto K = std::extent<T1, 1>::value;
        constexpr auto N = std::extent<T2, 1>::value;

        static_assert(K == std::extent<T2, 0>::value, "Inner dimensions must match");

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                uint32_t sum = 0;
                for (int k = 0; k < K; k++) {
                    sum += static_cast<uint32_t>(a[i][k]) * static_cast<uint32_t>(b[k][j]);
                }
                c[i][j] = sum;
            }
        }
    }

    template<typename T1, typename T2>
    bool matrix_equal(const T1& a, const T2& b) {
        constexpr auto M = std::extent<T1, 0>::value;
        constexpr auto N = std::extent<T1, 1>::value;

        static_assert(M == std::extent<T2, 0>::value && N == std::extent<T2, 1>::value, "Matrix dimensions must match");

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (a[i][j] != b[i][j]) {
                    printf("a[%d][%d] = %u, b[%d][%d] = %u\n", i, j, a[i][j], i, j, b[i][j]);
                    return false;
                }
            }
        }
        return true;
    }
}

using A_TYPE = MatmulType<TPosition::GM, CubeFormat::ND, uint16_t>;
using B_TYPE = MatmulType<TPosition::GM, CubeFormat::ND, uint16_t>;
using C_TYPE = MatmulType<TPosition::GM, CubeFormat::ND, uint32_t>;
using BIAS_TYPE = MatmulType<TPosition::GM, CubeFormat::ND, uint16_t>;

SCENARIO("Matmul Test") 
{
    typename A_TYPE::T a[CFG_NORM.singleCoreM][CFG_NORM.singleCoreK] = {
        { 1, 2, 3, 4, 5, 6 },
        { 2, 2, 3, 4, 5, 6 },
        { 3, 2, 3, 4, 5, 6 },
        { 4, 2, 3, 4, 5, 6 },
        { 5, 2, 3, 4, 5, 6 },
        { 6, 2, 3, 4, 5, 6 },
        { 7, 2, 3, 4, 5, 6 },
        { 8, 2, 3, 4, 5, 6 }
    };

    typename B_TYPE::T b[CFG_NORM.singleCoreK][CFG_NORM.singleCoreN] = {
        {1, 2, 3, 4, 5, 6, 7, 8},
        {2, 2, 3, 4, 5, 6, 7, 8},
        {3, 2, 3, 4, 5, 6, 7, 8},
        {4, 2, 3, 4, 5, 6, 7, 8},
        {5, 2, 3, 4, 5, 6, 7, 8},
        {6, 2, 3, 4, 5, 6, 7, 8}
    };

    typename C_TYPE::T c[CFG_NORM.singleCoreM][CFG_NORM.singleCoreN] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    typename C_TYPE::T expect_c[CFG_NORM.singleCoreM][CFG_NORM.singleCoreN];
    matrix_multiply(a, b, expect_c); 

    GlobalTensor<typename A_TYPE::T> tensorA;
    GlobalTensor<typename B_TYPE::T> tensorB;
    GlobalTensor<typename C_TYPE::T> tensorC;

    tensorA.SetAddr(*a, CFG_NORM.singleCoreM * CFG_NORM.singleCoreK);
    tensorB.SetAddr(*b, CFG_NORM.singleCoreK * CFG_NORM.singleCoreN);
    tensorC.SetAddr(*c, CFG_NORM.singleCoreM * CFG_NORM.singleCoreN);

    TCubeTiling tiling;

    GIVEN("MatmulImpl default") 
    {
        MatmulImpl<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE> mm;
        mm.Init(&tiling);

        WHEN("Set input tensors") 
        {
            mm.SetTensorA(tensorA);
            mm.SetTensorB(tensorB);

            THEN("Execute Matmul") 
            {
                mm.IterateAll(tensorC);
                REQUIRE(matrix_equal(c, expect_c));
            }
        }
        
        mm.End();

        DfxStatisticHandler::Print();
    }
}
