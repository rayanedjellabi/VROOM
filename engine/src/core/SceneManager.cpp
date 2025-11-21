#include "vroom/core/SceneManager.hpp"
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>

namespace vroom {

SceneManager::SceneManager() {
    // Initialize with a default empty scene
    m_activeScene = std::make_shared<Scene>();
    m_scenes.push_back(m_activeScene);
}

SceneManager::~SceneManager() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_scenes.clear();
    m_activeScene.reset();
}

void SceneManager::loadScene(const std::string& path) {
    auto newScene = createSceneFromFile(path);
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_scenes.clear();
    m_scenes.push_back(newScene);
    m_activeScene = newScene;
}

std::future<void> SceneManager::loadSceneAsync(const std::string& path) {
    return std::async(std::launch::async, [this, path]() {
        // Load the scene in the background thread
        auto newScene = createSceneFromFile(path);
        
        // Lock only when swapping the scenes
        std::lock_guard<std::mutex> lock(m_mutex);
        m_scenes.clear();
        m_scenes.push_back(newScene);
        m_activeScene = newScene;
    });
}

void SceneManager::loadSceneAdditive(const std::string& path) {
    auto newScene = createSceneFromFile(path);
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_scenes.push_back(newScene);
    // If no active scene, make this one active
    if (!m_activeScene) {
        m_activeScene = newScene;
    }
}

std::future<void> SceneManager::loadSceneAdditiveAsync(const std::string& path) {
    return std::async(std::launch::async, [this, path]() {
        auto newScene = createSceneFromFile(path);
        
        std::lock_guard<std::mutex> lock(m_mutex);
        m_scenes.push_back(newScene);
        if (!m_activeScene) {
            m_activeScene = newScene;
        }
    });
}

void SceneManager::unloadScene(std::shared_ptr<Scene> scene) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = std::remove(m_scenes.begin(), m_scenes.end(), scene);
    if (it != m_scenes.end()) {
        m_scenes.erase(it, m_scenes.end());
        
        if (m_activeScene == scene) {
            if (!m_scenes.empty()) {
                m_activeScene = m_scenes.back(); // Default to the last added scene
            } else {
                m_activeScene.reset();
            }
        }
    }
}

void SceneManager::update(float deltaTime) {
    std::lock_guard<std::mutex> lock(m_mutex);
    // We iterate over a copy or index because update might not be reentrant if scenes are modified during update
    // But since we lock, we are safe from external threads. 
    // However, if scene->update() calls something that calls SceneManager, we might deadlock if recursive.
    // Assuming Scene::update doesn't call SceneManager functions that lock.
    for (auto& scene : m_scenes) {
        if (scene) {
            scene->update(deltaTime);
        }
    }
}

std::shared_ptr<Scene> SceneManager::getActiveScene() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_activeScene;
}

std::shared_ptr<Scene> SceneManager::createSceneFromFile(const std::string& path) {
    // TODO: Implement actual file loading/deserialization logic here.
    // For now, we just return a new empty scene.
    // We could populate it with some test entities based on 'path' for demonstration.
    
    auto scene = std::make_shared<Scene>();
    // Example: scene->createEntity();
    return scene;
}

} // namespace vroom

