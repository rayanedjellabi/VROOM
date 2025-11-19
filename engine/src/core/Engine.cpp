#include "vroom/core/Engine.hpp"
#include "vroom/logging/LogMacros.hpp"

namespace vroom {

Engine::Engine() {
    LOG_ENGINE_INFO("Logger works!");
}

Engine::~Engine() {
}

}