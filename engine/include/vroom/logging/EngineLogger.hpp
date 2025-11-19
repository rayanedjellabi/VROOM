#pragma once

#include "vroom/logging/Logger.hpp"
#include <string>

namespace vroom {
namespace logging {

class EngineLogger {
public:
    // Log with class name
    void Debug(const std::string& className, const std::string& message);
    void Info(const std::string& className, const std::string& message);
    void Warning(const std::string& className, const std::string& message);
    void Error(const std::string& className, const std::string& message);

    // Get singleton instance
    static EngineLogger& GetInstance();

private:
    EngineLogger() = default;
    ~EngineLogger() = default;
    EngineLogger(const EngineLogger&) = delete;
    EngineLogger& operator=(const EngineLogger&) = delete;
};

} // namespace logging
} // namespace vroom

