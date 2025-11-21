#pragma once

#include "vroom/logging/EngineLogger.hpp"
#include "vroom/logging/ApplicationLogger.hpp"
#include <string>
#include <typeinfo>

#ifdef __GNUC__
#include <cxxabi.h>
#endif

namespace vroom {
namespace logging {

// Helper function to demangle class names
inline std::string getClassName(const std::type_info& typeInfo) {
#ifdef __GNUC__
    int status = 0;
    char* demangled = abi::__cxa_demangle(typeInfo.name(), nullptr, nullptr, &status);
    if (status == 0 && demangled) {
        std::string result(demangled);
        free(demangled);
        // Extract just the class name (remove namespace and template parameters if needed)
        size_t lastColon = result.find_last_of("::");
        if (lastColon != std::string::npos) {
            result = result.substr(lastColon + 1);
        }
        // Remove template parameters
        size_t templateStart = result.find('<');
        if (templateStart != std::string::npos) {
            result = result.substr(0, templateStart);
        }
        return result;
    }
#endif
    // Fallback: return mangled name if demangling fails or not available
    return typeInfo.name();
}

} // namespace logging
} // namespace vroom


#define EXPAND(x)                           x
#define LOGGER_GET_MACRO(_1, _2, name, ...)    name


// Engine logging macros that extract class name automatically from 'this' pointer
// Usage: VROOM_ENGINE_LOG_DEBUG_CLASS("My message");
// Note: These can only be used inside non-static member functions
#define LOG_ENGINE_CLASS_DEBUG(message) \
    vroom::logging::EngineLogger::getInstance().debug(vroom::logging::getClassName(typeid(*this)), message)

#define LOG_ENGINE_CLASS_INFO(message) \
    vroom::logging::EngineLogger::getInstance().info(vroom::logging::getClassName(typeid(*this)), message)

#define LOG_ENGINE_CLASS_WARNING(message) \
    vroom::logging::EngineLogger::getInstance().warning(vroom::logging::getClassName(typeid(*this)), message)

#define LOG_ENGINE_CLASS_ERROR(message) \
    vroom::logging::EngineLogger::getInstance().error(vroom::logging::getClassName(typeid(*this)), message)

#define LOG_ENGINE_STATIC_DEBUG(className, message) \
    vroom::logging::EngineLogger::getInstance().debug(className, message)

#define LOG_ENGINE_STATIC_INFO(className, message) \
    vroom::logging::EngineLogger::getInstance().info(className, message)

#define LOG_ENGINE_STATIC_WARNING(className, message) \
    vroom::logging::EngineLogger::getInstance().warning(className, message)

#define LOG_ENGINE_STATIC_ERROR(className, message) \
    vroom::logging::EngineLogger::getInstance().error(className, message)

#define LOG_APP_CLASS_DEBUG(message) \
    vroom::logging::ApplicationLogger::getInstance().debug(vroom::logging::getClassName(typeid(*this)), message)

#define LOG_APP_CLASS_INFO(message) \
    vroom::logging::ApplicationLogger::getInstance().info(vroom::logging::getClassName(typeid(*this)), message)

#define LOG_APP_CLASS_WARNING(message) \
    vroom::logging::ApplicationLogger::getInstance().warning(vroom::logging::getClassName(typeid(*this)), message)

#define LOG_APP_CLASS_ERROR(message) \
    vroom::logging::ApplicationLogger::getInstance().error(vroom::logging::getClassName(typeid(*this)), message)

#define LOG_APP_STATIC_DEBUG(className, message) \
    vroom::logging::ApplicationLogger::getInstance().debug(className, message)

#define LOG_APP_STATIC_INFO(className, message) \
    vroom::logging::ApplicationLogger::getInstance().info(className, message)

#define LOG_APP_STATIC_WARNING(className, message) \
    vroom::logging::ApplicationLogger::getInstance().warning(className, message)

#define LOG_APP_STATIC_ERROR(className, message) \
    vroom::logging::ApplicationLogger::getInstance().error(className, message)

#define LOG_ENGINE_DEBUG(...) \
    EXPAND( LOGGER_GET_MACRO(__VA_ARGS__, LOG_ENGINE_STATIC_DEBUG, LOG_ENGINE_CLASS_DEBUG)(__VA_ARGS__) )

#define LOG_ENGINE_INFO(...) \
    EXPAND( LOGGER_GET_MACRO(__VA_ARGS__, LOG_ENGINE_STATIC_INFO, LOG_ENGINE_CLASS_INFO)(__VA_ARGS__) )

#define LOG_ENGINE_WARNING(...) \
    EXPAND( LOGGER_GET_MACRO(__VA_ARGS__, LOG_ENGINE_STATIC_WARNING, LOG_ENGINE_CLASS_WARNING)(__VA_ARGS__) )

#define LOG_ENGINE_ERROR(...) \
    EXPAND( LOGGER_GET_MACRO(__VA_ARGS__, LOG_ENGINE_STATIC_ERROR, LOG_ENGINE_CLASS_ERROR)(__VA_ARGS__) )

#define LOG_DEBUG(...) \
    EXPAND( LOGGER_GET_MACRO(__VA_ARGS__, LOG_APP_STATIC_DEBUG, LOG_APP_CLASS_DEBUG)(__VA_ARGS__) )

#define LOG_INFO(...) \
    EXPAND( LOGGER_GET_MACRO(__VA_ARGS__, LOG_APP_STATIC_INFO, LOG_APP_CLASS_INFO)(__VA_ARGS__) )

#define LOG_WARNING(...) \
    EXPAND( LOGGER_GET_MACRO(__VA_ARGS__, LOG_APP_STATIC_WARNING, LOG_APP_CLASS_WARNING)(__VA_ARGS__) )

#define LOG_ERROR(...) \
    EXPAND( LOGGER_GET_MACRO(__VA_ARGS__, LOG_APP_STATIC_ERROR, LOG_APP_CLASS_ERROR)(__VA_ARGS__) )
