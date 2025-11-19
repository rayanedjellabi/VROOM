#pragma once

namespace vroom {
namespace logging {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

const char* LogLevelToString(LogLevel level);

} // namespace logging
} // namespace vroom

