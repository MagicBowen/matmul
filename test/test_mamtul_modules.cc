#include "catch2/catch.hpp"
#include "matmul_type_def.h"
#include "matmul_policy.h"
#include "matmul_config.h"

using namespace matmul;

SCENARIO("MMad single block test") {
    MMad<void, int16_t, int32_t, CFG_NORM> mmad;

    int16_t a[CFG_NORM.basicM * CFG_NORM.basicK] = {1, 2, 3, 4};
    int16_t b[CFG_NORM.basicK * CFG_NORM.basicN] = {2, 3, 4, 2};
    int32_t c[CFG_NORM.basicM * CFG_NORM.basicN] = {0};

    LocalTensor<int16_t> tensorA;
    LocalTensor<int16_t> tensorB;
    LocalTensor<int32_t> tensorC;

    tensorA.SetAddr(a, CFG_NORM.basicM * CFG_NORM.basicK);
    tensorB.SetAddr(b, CFG_NORM.basicK * CFG_NORM.basicN);
    tensorC.SetAddr(c, CFG_NORM.basicM * CFG_NORM.basicN);

    mmad.Compute(tensorC, tensorA, tensorB);

    REQUIRE(c[0] == 1 * 2 + 2 * 4);
    REQUIRE(c[1] == 1 * 3 + 2 * 2);
    REQUIRE(c[2] == 3 * 2 + 4 * 4);
    REQUIRE(c[3] == 3 * 3 + 4 * 2);
}
