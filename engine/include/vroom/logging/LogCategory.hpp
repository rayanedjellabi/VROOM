#pragma once

namespace vroom {
namespace logging {

enum class LogCategory {
    Engine,
    Application
};

const char* LogCategoryToString(LogCategory category);

} // namespace logging
} // namespace vroom

