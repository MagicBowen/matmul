#include "catch2/catch.hpp"
#include "matmul_impl.h"

using namespace matmul;

using A_TYPE = matmul::MatmulType<TPosition::TSCM, CubeFormat::ND, half>;
using B_TYPE = matmul::MatmulType<TPosition::TSCM, CubeFormat::ND, half>;
using C_TYPE = matmul::MatmulType<TPosition::GM, CubeFormat::ND, float>;
using BIAS_TYPE = matmul::MatmulType<TPosition::GM, CubeFormat::ND, float>;

SCENARIO("Matmul Sample Test") 
{
    TCubeTiling tiling;

    GIVEN("MatmulImpl Default") 
    {
        MatmulImpl<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE> mm;

        WHEN("DFX statistic handler is cleared") 
        {
            MatmulDfxStatisticHandler::Clear();

            THEN("All API calls should be printed") 
            {
                mm.Init(&tiling);

                mm.SetTensorA(GlobalTensor<A_TYPE::T>{});
                mm.SetTensorB(GlobalTensor<B_TYPE::T>{});

                LocalTensor<typename C_TYPE::T> tensorC;
                mm.IterateAll(tensorC);
                mm.End();
            }

            MatmulDfxStatisticHandler::PrintStatistic();
        }
    }
}
