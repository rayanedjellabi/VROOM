#pragma once

#include "vroom/logging/Logger.hpp"
#include <string>

namespace vroom {
namespace logging {

/// \brief Specialized logger for engine-level events.
class EngineLogger {
public:
    /// \brief Logs a debug message with class context.
    /// \param className The name of the class.
    /// \param message The log message.
    void debug(const std::string& className, const std::string& message);

    /// \brief Logs an info message with class context.
    /// \param className The name of the class.
    /// \param message The log message.
    void info(const std::string& className, const std::string& message);

    /// \brief Logs a warning message with class context.
    /// \param className The name of the class.
    /// \param message The log message.
    void warning(const std::string& className, const std::string& message);

    /// \brief Logs an error message with class context.
    /// \param className The name of the class.
    /// \param message The log message.
    void error(const std::string& className, const std::string& message);

    /// \brief Gets the singleton instance of the EngineLogger.
    /// \return Reference to the EngineLogger instance.
    static EngineLogger& getInstance();

private:
    EngineLogger() = default;
    ~EngineLogger() = default;
    EngineLogger(const EngineLogger&) = delete;
    EngineLogger& operator=(const EngineLogger&) = delete;
};

} // namespace logging
} // namespace vroom
