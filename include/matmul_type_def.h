/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_TYPE_DEF_H
#define MATMUL_TYPE_DEF_H

#include <stdint.h>

namespace matmul {

enum class TPosition : int32_t {
    GM,
    A1,
    A2,
    B1,
    B2,
    C1,
    C2,
    CO1,
    CO2,
    VECIN,
    VECOUT,
    VECCALC,
    LCM = VECCALC,
    SPM,
    SHM = SPM,
    TSCM,
    MAX,
};

enum class CubeFormat {
    ND = 0,
    NZ,
    ZN,
    ZZ,
    NN,
    ND_ALIGN,
    SCALAR,
    VECTOR,
};

enum class LayoutMode {
    NONE = 0,
    BSNGD,
    SBNGD,
    BNGS1S2,
    NORMAL
};

template <TPosition POSITION, CubeFormat FORMAT, typename TYPE, bool ISTRANS = false,
          LayoutMode LAYOUT = LayoutMode::NONE, bool IBSHARE = false>
struct MatmulType {
    constexpr static TPosition pos = POSITION;
    constexpr static CubeFormat format = FORMAT;
    using T = TYPE;
    constexpr static bool isTrans = ISTRANS;
    constexpr static LayoutMode layout = LAYOUT;
    constexpr static bool ibShare = IBSHARE;
};

using half = _Float16;

constexpr bool PhyPosIsL1(TPosition pos)
{
    if (pos == TPosition::A1 || pos == TPosition::B1 ||
        pos == TPosition::SHM || pos == TPosition::TSCM) {
        return true;
    }
    return false;
}


}

#endif
