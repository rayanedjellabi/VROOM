#pragma once

#include <memory>
#include <string>
#include <vector>
#include <future>
#include <mutex>
#include "vroom/core/Scene.hpp"

namespace vroom {

class SceneManager : public std::enable_shared_from_this<SceneManager> {
public:
    SceneManager();
    ~SceneManager();

    /// \brief Loads a scene synchronously, unloading all currently loaded scenes.
    /// \param path Path to the scene file (or identifier).
    void loadScene(const std::string& path);

    /// \brief Loads an existing scene object, unloading all currently loaded scenes.
    /// \param scene The scene object to load.
    void loadScene(std::shared_ptr<Scene> scene);

    /// \brief Loads a scene asynchronously, unloading all currently loaded scenes.
    /// \param path Path to the scene file (or identifier).
    /// \return A future that completes when the scene is loaded.
    std::future<void> loadSceneAsync(const std::string& path);

    /// \brief Loads a scene additively synchronously.
    /// The new scene is added to the list of active scenes.
    /// \param path Path to the scene file (or identifier).
    void loadSceneAdditive(const std::string& path);

    /// \brief Loads a scene additively asynchronously.
    /// \param path Path to the scene file (or identifier).
    /// \return A future that completes when the scene is loaded.
    std::future<void> loadSceneAdditiveAsync(const std::string& path);

    /// \brief Unloads a specific scene.
    /// \param scene The scene to unload.
    void unloadScene(std::shared_ptr<Scene> scene);

    /// \brief Updates all active scenes.
    /// \param deltaTime Time elapsed since the last frame.
    void update(float deltaTime);

    /// \brief Gets the currently active (primary) scene.
    /// \return Shared pointer to the active scene.
    std::shared_ptr<Scene> getActiveScene() const;

private:
    std::vector<std::shared_ptr<Scene>> m_scenes;
    std::shared_ptr<Scene> m_activeScene;
    mutable std::mutex m_mutex;

protected:
    // Internal helper for loading logic, protected for testing
    virtual std::shared_ptr<Scene> createSceneFromFile(const std::string& path);
};


} // namespace vroom

