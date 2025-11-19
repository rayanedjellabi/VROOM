#include "vroom/logging/Logger.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include "vroom/logging/LogLevel.hpp"

namespace vroom {
namespace logging {

Logger::Logger() 
    : m_engineStream(&std::cout)
    , m_applicationStream(&std::cout)
    , m_engineErrorStream(&std::cerr)
    , m_applicationErrorStream(&std::cerr) {
}

Logger::~Logger() {
    // Note: We don't delete streams here as the caller manages their lifetime
    // Streams passed to SetEngineStream/SetApplicationStream must remain valid
    // until ResetEngineStream/ResetApplicationStream is called or Logger is destroyed
}

void Logger::SetEngineStream(std::ostream* stream) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_engineStream = stream ? stream : &std::cout;
}

void Logger::SetApplicationStream(std::ostream* stream) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_applicationStream = stream ? stream : &std::cout;
}

void Logger::SetEngineErrorStream(std::ostream* stream) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_engineErrorStream = stream ? stream : &std::cerr;
}

void Logger::SetApplicationErrorStream(std::ostream* stream) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_applicationErrorStream = stream ? stream : &std::cerr;
}

void Logger::ResetEngineStream() {
    SetEngineStream(&std::cout);
}

void Logger::ResetApplicationStream() {
    SetApplicationStream(&std::cout);
}

void Logger::ResetEngineErrorStream() {
    SetEngineErrorStream(&std::cerr);
}

void Logger::ResetApplicationErrorStream() {
    SetApplicationErrorStream(&std::cerr);
}

std::ostream* Logger::GetStream(LogLevel level, LogCategory category) {
    // Error level logs go to error streams, others go to normal streams
    if (level == LogLevel::Error) {
        switch (category) {
            case LogCategory::Engine:
                return m_engineErrorStream;
            case LogCategory::Application:
                return m_applicationErrorStream;
            default:
                return &std::cerr;
        }
    } else {
        switch (category) {
            case LogCategory::Engine:
                return m_engineStream;
            case LogCategory::Application:
                return m_applicationStream;
            default:
                return &std::cout;
        }
    }
}

std::string Logger::FormatMessage(LogLevel level, LogCategory category, const std::string& className, const std::string& message) {
    std::ostringstream oss;
    
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::tm* tm_info = std::localtime(&time);
    
    // Format: [HH:MM:SS.mmm] [CATEGORY] [LEVEL] [ClassName] message
    oss << "[" 
        << std::setfill('0') << std::setw(2) << tm_info->tm_hour << ":"
        << std::setfill('0') << std::setw(2) << tm_info->tm_min << ":"
        << std::setfill('0') << std::setw(2) << tm_info->tm_sec << "."
        << std::setfill('0') << std::setw(3) << ms.count()
        << "] [" << LogCategoryToString(category) << "] "
        << "[" << LogLevelToString(level) << "] "
        << "[" << className << "] "
        << message;
    
    return oss.str();
}

void Logger::Log(LogLevel level, LogCategory category, const std::string& className, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::ostream* stream = GetStream(level, category);
    *stream << FormatMessage(level, category, className, message) << std::endl;
    stream->flush();
}

void Logger::Debug(LogCategory category, const std::string& className, const std::string& message) {
    Log(LogLevel::Debug, category, className, message);
}

void Logger::Info(LogCategory category, const std::string& className, const std::string& message) {
    Log(LogLevel::Info, category, className, message);
}

void Logger::Warning(LogCategory category, const std::string& className, const std::string& message) {
    Log(LogLevel::Warning, category, className, message);
}

void Logger::Error(LogCategory category, const std::string& className, const std::string& message) {
    Log(LogLevel::Error, category, className, message);
}

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

} // namespace logging
} // namespace vroom

