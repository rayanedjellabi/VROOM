#include "vroom/logging/EngineLogger.hpp"

namespace vroom {
namespace logging {

void EngineLogger::Debug(const std::string& className, const std::string& message) {
    Logger::GetInstance().Debug(LogCategory::Engine, className, message);
}

void EngineLogger::Info(const std::string& className, const std::string& message) {
    Logger::GetInstance().Info(LogCategory::Engine, className, message);
}

void EngineLogger::Warning(const std::string& className, const std::string& message) {
    Logger::GetInstance().Warning(LogCategory::Engine, className, message);
}

void EngineLogger::Error(const std::string& className, const std::string& message) {
    Logger::GetInstance().Error(LogCategory::Engine, className, message);
}

EngineLogger& EngineLogger::GetInstance() {
    static EngineLogger instance;
    return instance;
}

} // namespace logging
} // namespace vroom

