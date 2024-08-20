#include "catch2/catch.hpp"
#include "matmul_impl.h"

using namespace matmul;

namespace {
    template<typename IMPL, typename INPUT_TYPE, const auto& MM_CFG, typename = void>
    class CustomizedCopyCubeIn : public matmul::CopyCubeIn<IMPL, INPUT_TYPE, MM_CFG> {
        MATMUL_USE_MODULE(Context);
        using SrcT = typename INPUT_TYPE::INNER_T;
    public:
        using BASE_MODULE = matmul::CopyCubeIn<IMPL, INPUT_TYPE, MM_CFG>;
    public:
        LocalTensor<SrcT> Load(uint32_t row, uint32_t col) {
            LocalTensor<SrcT> result;
            std::cout << "CustomizedCopyCubeIn::Load" << std::endl;
            std::cout << "Context value = " << MATMUL_MODULE(Context)->value  << std::endl;
            return result;
        }
    };

    template<typename IMPL, typename INPUT_TYPE, const auto& MM_CFG>
    class CustomizedCopyCubeIn<IMPL, INPUT_TYPE, MM_CFG, std::enable_if_t<PhyPosIsL1(INPUT_TYPE::pos)>> 
    : public matmul::CopyCubeIn<IMPL, INPUT_TYPE, MM_CFG> {
        MATMUL_USE_MODULE(Context);
        using SrcT = typename INPUT_TYPE::INNER_T;
    public:
        using BASE_MODULE = matmul::CopyCubeIn<IMPL, INPUT_TYPE, MM_CFG>;
    public:
        LocalTensor<SrcT> Load(uint32_t row, uint32_t col) {
            LocalTensor<SrcT> result;
            std::cout << "CustomizedCopyCubeIn::Load, in partial specialized version" << std::endl;
            std::cout << "Context value = " << MATMUL_MODULE(Context)->value  << std::endl;
            return result;
        }
    };

    struct CustomizedMatmulContext {
        uint32_t value{0xABCD};
    };

    template<const auto& MM_CFG, typename IMPL, typename A_TYPE, typename B_TYPE, typename C_TYPE, typename BIAS_TYPE>
    struct MatmulPolicyCustomized : MatmulPolicyDefault<MM_CFG, IMPL, A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE> {
        using CopyCubeInA = CustomizedCopyCubeIn<IMPL, MatmulInputTypeA<A_TYPE>, MM_CFG>;
        using Context     = CustomizedMatmulContext;
    };
}

using A_TYPE = MatmulType<TPosition::A1, CubeFormat::ND, uint16_t>;
using B_TYPE = MatmulType<TPosition::GM, CubeFormat::ND, uint16_t>;
using C_TYPE = MatmulType<TPosition::GM, CubeFormat::ND, uint32_t>;
using BIAS_TYPE = MatmulType<TPosition::GM, CubeFormat::ND, uint16_t>;

SCENARIO("matmul extention test") {

    GlobalTensor<typename A_TYPE::T> tensorA;
    GlobalTensor<typename B_TYPE::T> tensorB;
    GlobalTensor<typename C_TYPE::T> tensorC;

    TCubeTiling tiling;

    GIVEN("MatmulImpl customized") 
    {
        MatmulImpl<A_TYPE, B_TYPE, C_TYPE, BIAS_TYPE, CFG_NORM, MatmulPolicyCustomized> mm;
                   
        mm.Init(&tiling);

        WHEN("Set input tensors") 
        {
            mm.SetTensorA(tensorA);
            mm.SetTensorB(tensorB);

            THEN("Execute Matmul") 
            {
                mm.IterateAll(tensorC);
            }
        }
        
        mm.End();

        MatmulDfxStatisticHandler::PrintStatistic();
    }
}
