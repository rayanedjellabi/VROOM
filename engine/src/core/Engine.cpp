#include "vroom/core/Engine.hpp"
#include "vroom/logging/LogMacros.hpp"
#include "vroom/core/Version.hpp"
#include <chrono>

namespace vroom {

Engine::Engine() 
    : m_isRunning(false) {
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

void Engine::run() {
    m_isRunning = true;
    auto lastTime = std::chrono::high_resolution_clock::now();

    LOG_ENGINE_INFO("Starting engine loop...");

    while (m_isRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        update(deltaTime);
    }

    LOG_ENGINE_INFO("Engine loop stopped.");
}

void Engine::stop() {
    m_isRunning = false;
}

} // namespace vroom
