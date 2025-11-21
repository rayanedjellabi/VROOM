#include "vroom/logging/EngineLogger.hpp"

namespace vroom {
namespace logging {

void EngineLogger::debug(const std::string& className, const std::string& message) {
    Logger::getInstance().debug(LogCategory::Engine, className, message);
}

void EngineLogger::info(const std::string& className, const std::string& message) {
    Logger::getInstance().info(LogCategory::Engine, className, message);
}

void EngineLogger::warning(const std::string& className, const std::string& message) {
    Logger::getInstance().warning(LogCategory::Engine, className, message);
}

void EngineLogger::error(const std::string& className, const std::string& message) {
    Logger::getInstance().error(LogCategory::Engine, className, message);
}

EngineLogger& EngineLogger::getInstance() {
    static EngineLogger instance;
    return instance;
}

} // namespace logging
} // namespace vroom
