#pragma once

#include "vroom/logging/Logger.hpp"
#include <string>

namespace vroom {
namespace logging {

class ApplicationLogger {
public:
    // Log with class name
    void Debug(const std::string& className, const std::string& message);
    void Info(const std::string& className, const std::string& message);
    void Warning(const std::string& className, const std::string& message);
    void Error(const std::string& className, const std::string& message);

    // Get singleton instance
    static ApplicationLogger& GetInstance();

private:
    ApplicationLogger() = default;
    ~ApplicationLogger() = default;
    ApplicationLogger(const ApplicationLogger&) = delete;
    ApplicationLogger& operator=(const ApplicationLogger&) = delete;
};

} // namespace logging
} // namespace vroom

