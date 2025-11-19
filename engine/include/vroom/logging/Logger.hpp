#pragma once

#include <ostream>
#include <memory>
#include <mutex>
#include <string>
#include "vroom/logging/LogLevel.hpp"
#include "vroom/logging/LogCategory.hpp"

namespace vroom {
namespace logging {

class Logger {
public:
    Logger();
    ~Logger();

    // Set output streams for different categories
    void SetEngineStream(std::ostream* stream);
    void SetApplicationStream(std::ostream* stream);
    
    // Set error streams (defaults to stderr)
    void SetEngineErrorStream(std::ostream* stream);
    void SetApplicationErrorStream(std::ostream* stream);
    
    // Reset streams to default (std::cout for normal, std::cerr for errors)
    void ResetEngineStream();
    void ResetApplicationStream();
    void ResetEngineErrorStream();
    void ResetApplicationErrorStream();

    // Log with class name
    void Log(LogLevel level, LogCategory category, const std::string& className, const std::string& message);
    
    // Convenience methods
    void Debug(LogCategory category, const std::string& className, const std::string& message);
    void Info(LogCategory category, const std::string& className, const std::string& message);
    void Warning(LogCategory category, const std::string& className, const std::string& message);
    void Error(LogCategory category, const std::string& className, const std::string& message);

    // Get singleton instance
    static Logger& GetInstance();

private:
    std::ostream* m_engineStream;
    std::ostream* m_applicationStream;
    std::ostream* m_engineErrorStream;
    std::ostream* m_applicationErrorStream;
    std::mutex m_mutex;

    std::ostream* GetStream(LogLevel level, LogCategory category);
    std::string FormatMessage(LogLevel level, LogCategory category, const std::string& className, const std::string& message);
};

} // namespace logging
} // namespace vroom

