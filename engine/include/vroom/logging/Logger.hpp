#pragma once

#include <ostream>
#include <memory>
#include <mutex>
#include <string>
#include "vroom/logging/LogLevel.hpp"
#include "vroom/logging/LogCategory.hpp"

namespace vroom {
namespace logging {

/// \brief Central logger class handling stream outputs for different categories and levels.
class Logger {
public:
    Logger();
    ~Logger();

    /// \brief Sets the output stream for engine logs.
    /// \param stream Pointer to the output stream (e.g., &std::cout).
    void setEngineStream(std::ostream* stream);

    /// \brief Sets the output stream for application logs.
    /// \param stream Pointer to the output stream (e.g., &std::cout).
    void setApplicationStream(std::ostream* stream);
    
    /// \brief Sets the error output stream for engine logs.
    /// \param stream Pointer to the error output stream (e.g., &std::cerr).
    void setEngineErrorStream(std::ostream* stream);

    /// \brief Sets the error output stream for application logs.
    /// \param stream Pointer to the error output stream (e.g., &std::cerr).
    void setApplicationErrorStream(std::ostream* stream);
    
    /// \brief Resets the engine output stream to default (std::cout).
    void resetEngineStream();

    /// \brief Resets the application output stream to default (std::cout).
    void resetApplicationStream();

    /// \brief Resets the engine error output stream to default (std::cerr).
    void resetEngineErrorStream();

    /// \brief Resets the application error output stream to default (std::cerr).
    void resetApplicationErrorStream();

    /// \brief Logs a message with specific level, category, and class context.
    /// \param level The log level (Debug, Info, etc.).
    /// \param category The log category (Engine or Application).
    /// \param className The name of the class where the log originates.
    /// \param message The log message.
    void log(LogLevel level, LogCategory category, const std::string& className, const std::string& message);
    
    /// \brief Convenience method for logging debug messages.
    /// \param category The log category.
    /// \param className The name of the class.
    /// \param message The log message.
    void debug(LogCategory category, const std::string& className, const std::string& message);

    /// \brief Convenience method for logging info messages.
    /// \param category The log category.
    /// \param className The name of the class.
    /// \param message The log message.
    void info(LogCategory category, const std::string& className, const std::string& message);

    /// \brief Convenience method for logging warning messages.
    /// \param category The log category.
    /// \param className The name of the class.
    /// \param message The log message.
    void warning(LogCategory category, const std::string& className, const std::string& message);

    /// \brief Convenience method for logging error messages.
    /// \param category The log category.
    /// \param className The name of the class.
    /// \param message The log message.
    void error(LogCategory category, const std::string& className, const std::string& message);

    /// \brief Gets the singleton instance of the Logger.
    /// \return Reference to the Logger instance.
    static Logger& getInstance();

private:
    std::ostream* m_engineStream;
    std::ostream* m_applicationStream;
    std::ostream* m_engineErrorStream;
    std::ostream* m_applicationErrorStream;
    std::mutex m_mutex;

    std::ostream* getStream(LogLevel level, LogCategory category);
    std::string formatMessage(LogLevel level, LogCategory category, const std::string& className, const std::string& message);
};

} // namespace logging
} // namespace vroom
