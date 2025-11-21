#pragma once

#include "vroom/logging/Logger.hpp"
#include <string>

namespace vroom {
namespace logging {

/// \brief Specialized logger for application-level events.
class ApplicationLogger {
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

    /// \brief Gets the singleton instance of the ApplicationLogger.
    /// \return Reference to the ApplicationLogger instance.
    static ApplicationLogger& getInstance();

private:
    ApplicationLogger() = default;
    ~ApplicationLogger() = default;
    ApplicationLogger(const ApplicationLogger&) = delete;
    ApplicationLogger& operator=(const ApplicationLogger&) = delete;
};

} // namespace logging
} // namespace vroom
