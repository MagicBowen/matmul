#include "dfx/handlers/dfx_statistic_handler.h"

namespace matmul {
    std::unordered_map<std::string, uint32_t> DfxStatisticHandler::funcCallCounts_;
}