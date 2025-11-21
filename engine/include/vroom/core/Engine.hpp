#pragma once

#include <memory>
#include "vroom/core/SceneManager.hpp"

namespace vroom {

class Engine {
public:
    Engine();
    ~Engine();

    /// \brief Updates the engine systems.
    /// \param deltaTime Time elapsed since the last frame.
    void update(float deltaTime);

    /// \brief Runs the main engine loop.
    void run();

    /// \brief Stops the engine loop.
    void stop();

    /// \brief Gets the scene manager.
    /// \return Reference to the scene manager.
    SceneManager& getSceneManager() { return *m_sceneManager; }

private:
    std::shared_ptr<SceneManager> m_sceneManager;
    bool m_isRunning;
};

} // namespace vroom
