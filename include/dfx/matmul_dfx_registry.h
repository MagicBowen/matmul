/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MATMUL_DFX_REGISTRY_H
#define MATMUL_DFX_REGISTRY_H

#include "dfx/matmul_dfx_proxy.h"

namespace matmul {

MATMUL_DFX_PROXY_REGISTER(Scheduler, Init, Destroy, ScheduleOnce, Schedule);
MATMUL_DFX_PROXY_REGISTER(CopyCubeInA, Init, Destroy, SetAddr, Load, Clear);
MATMUL_DFX_PROXY_REGISTER(CopyCubeInB, Init, Destroy, SetAddr, Load, Clear);
MATMUL_DFX_PROXY_REGISTER(CopyInBufferA, Init, Destroy, Alloc, Free, Get);
MATMUL_DFX_PROXY_REGISTER(CopyInBufferB, Init, Destroy, Alloc, Free, Get);
MATMUL_DFX_PROXY_REGISTER(MMad, Compute);
MATMUL_DFX_PROXY_REGISTER(Co1Buffer, Init, Destroy, Alloc, Free, Get);
MATMUL_DFX_PROXY_REGISTER(CopyCubeOut, Copy);
// MATMUL_DFX_PROXY_REGISTER(HalInstruction, CopyND2NZ);

}

#endif
