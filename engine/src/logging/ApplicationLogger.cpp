#include "vroom/logging/ApplicationLogger.hpp"

namespace vroom {
namespace logging {

void ApplicationLogger::Debug(const std::string& className, const std::string& message) {
    Logger::GetInstance().Debug(LogCategory::Application, className, message);
}

void ApplicationLogger::Info(const std::string& className, const std::string& message) {
    Logger::GetInstance().Info(LogCategory::Application, className, message);
}

void ApplicationLogger::Warning(const std::string& className, const std::string& message) {
    Logger::GetInstance().Warning(LogCategory::Application, className, message);
}

void ApplicationLogger::Error(const std::string& className, const std::string& message) {
    Logger::GetInstance().Error(LogCategory::Application, className, message);
}

ApplicationLogger& ApplicationLogger::GetInstance() {
    static ApplicationLogger instance;
    return instance;
}

} // namespace logging
} // namespace vroom

