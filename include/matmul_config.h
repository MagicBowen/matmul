/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef MAMTUL_CONFIG_H
#define MAMTUL_CONFIG_H

#include <stdint.h>

namespace matmul {

enum IterateMode : uint8_t {
    ITERATE_MODE_NORMAL  = 0b00000001,
    ITERATE_MODE_ALL     = 0b00000010,
    ITERATE_MODE_BATCH   = 0b00000100,
    ITERATE_MODE_N_BATCH = 0b00001000,
    ITERATE_MODE_DEFAULT = 0b11111111,
};

enum class IterateOrder {
    ORDER_M = 0,
    ORDER_N,
    UNDEF,
};

enum class ScheduleType {
    INNER_PRODUCT = 0, // k loop, default type
    OUTER_PRODUCT,     // m/n loop, depends on IterateOrder
};

struct MatmulConfig {
    bool doNorm;
    bool doBasicBlock;
    bool doMultiDataLoad;
    bool isNBatch;
    uint32_t basicM;
    uint32_t basicN;
    uint32_t basicK;
    uint32_t doMTE2Preload;
    uint32_t singleCoreM;
    uint32_t singleCoreN;
    uint32_t singleCoreK;
    uint32_t stepM;
    uint32_t stepN;
    uint32_t baseMN;
    uint32_t singleCoreMN;
    bool enUnitFlag = true;

    uint8_t iterateMode = IterateMode::ITERATE_MODE_DEFAULT;
    IterateOrder iterateOrder = IterateOrder::ORDER_M;
    ScheduleType scheduleType;
};

constexpr inline MatmulConfig GetNormalConfig(const IterateOrder iterateOrder = IterateOrder::ORDER_M,
    const ScheduleType scheduleType = ScheduleType::INNER_PRODUCT, const bool enUnitFlag = true)
{
    return {
        .doNorm = true,
        .doBasicBlock = false,
        .doMultiDataLoad = false,
        .isNBatch = false,
        .basicM = 2,
        .basicN = 2,
        .basicK = 2,
        .doMTE2Preload = 1,
        .singleCoreM = 8,
        .singleCoreN = 8,
        .singleCoreK = 6,
        .stepM = 2,
        .stepN = 2,
        .baseMN = 4,
        .singleCoreMN = 64,
        .enUnitFlag = enUnitFlag,
        .iterateMode = IterateMode::ITERATE_MODE_DEFAULT,
        .iterateOrder = iterateOrder,
        .scheduleType = scheduleType,
    };
}

constexpr MatmulConfig CFG_NORM = GetNormalConfig();

}

#endif
