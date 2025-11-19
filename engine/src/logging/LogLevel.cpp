#include "vroom/logging/LogLevel.hpp"

namespace vroom {
namespace logging {

const char* LogLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARNING";
        case LogLevel::Error:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

} // namespace logging
} // namespace vroom

