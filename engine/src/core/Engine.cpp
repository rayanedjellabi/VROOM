#include "vroom/core/Engine.hpp"
#include "vroom/logging/LogMacros.hpp"
#include "vroom/core/Version.hpp"

namespace vroom {

Engine::Engine() {
    LOG_ENGINE_INFO("Initializing VROOM Engine v" + Version::getVersionString() + " (" + Version::GIT_HASH + ")");
    m_sceneManager = std::make_shared<SceneManager>();
}

Engine::~Engine() {
    m_sceneManager.reset();
    LOG_ENGINE_INFO("Engine shutdown complete, goodbye!");
}

void Engine::update(float deltaTime) {
    if (m_sceneManager) {
        m_sceneManager->update(deltaTime);
    }
}

} // namespace vroom
