#include "dfx/handlers/matmul_dfx_statistic_handler.h"

namespace matmul {
    std::unordered_map<std::string, uint32_t> MatmulDfxStatisticHandler::funcCallCounts_;
}