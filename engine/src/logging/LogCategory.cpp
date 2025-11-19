#include "vroom/logging/LogCategory.hpp"

namespace vroom {
namespace logging {

const char* LogCategoryToString(LogCategory category) {
    switch (category) {
        case LogCategory::Engine:
            return "VROOM";
        case LogCategory::Application:
            return "APP";
        default:
            return "UNKNOWN";
    }
}

} // namespace logging
} // namespace vroom

