#include "vroom/logging/ApplicationLogger.hpp"

namespace vroom {
namespace logging {

void ApplicationLogger::debug(const std::string& className, const std::string& message) {
    Logger::getInstance().debug(LogCategory::Application, className, message);
}

void ApplicationLogger::info(const std::string& className, const std::string& message) {
    Logger::getInstance().info(LogCategory::Application, className, message);
}

void ApplicationLogger::warning(const std::string& className, const std::string& message) {
    Logger::getInstance().warning(LogCategory::Application, className, message);
}

void ApplicationLogger::error(const std::string& className, const std::string& message) {
    Logger::getInstance().error(LogCategory::Application, className, message);
}

ApplicationLogger& ApplicationLogger::getInstance() {
    static ApplicationLogger instance;
    return instance;
}

} // namespace logging
} // namespace vroom
