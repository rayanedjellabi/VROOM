#include "vroom/core/Engine.hpp"
#include "vroom/logging/LogMacros.hpp"
#include "vroom/core/Version.hpp"

namespace vroom {

Engine::Engine() {
    LOG_ENGINE_INFO("Initializing VROOM Engine v" + Version::getVersionString() + " (" + Version::GIT_HASH + ")");
}

Engine::~Engine() {
    LOG_ENGINE_INFO("Engine shutdown complete, goodbye!");
}

} // namespace vroom
