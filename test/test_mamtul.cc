#include "catch2/catch.hpp"
#include "matmul_impl.h"

using namespace matmul;

using A_TYPE = matmul::MatmulType<TPosition::TSCM, CubeFormat::ND, half>;
using B_TYPE = matmul::MatmulType<TPosition::TSCM, CubeFormat::ND, half>;
using C_TYPE = matmul::MatmulType<TPosition::GM, CubeFormat::ND, float>;
using BIAS_TYPE = matmul::MatmulType<TPosition::GM, CubeFormat::ND, float>;

SCENARIO("Sample Test") {
    MatmulImpl<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE> mm;
    mm.SetTensorA(GlobalTensor<A_TYPE::T>{});
    mm.IterateAll(LocalTensor<float>{});
    REQUIRE(!mm.Iterate());
}
