#include "vroom/core/Engine.hpp"
#include "vroom/logging/LogMacros.hpp"
#include "vroom/core/Version.hpp"
#include "vroom/vulkan/VulkanRenderer.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <chrono>
#include <stdexcept>
#include <thread>

namespace vroom {

Engine::Engine(const EngineConfig& config) 
    : m_config(config), m_isRunning(false) {
    LOG_ENGINE_INFO("Initializing VROOM Engine v" + Version::getVersionString() + " (" + Version::GIT_HASH + ")");
    
    if (!m_config.headless) {
        initWindow();

        m_renderer = std::make_unique<VulkanRenderer>();
        try {
            m_renderer->init(m_window);
        } catch (const std::exception& e) {
            LOG_ENGINE_ERROR("Failed to initialize Vulkan renderer: " + std::string(e.what()));
            throw;
        }
    } else {
        LOG_ENGINE_INFO("Running in HEADLESS mode. Graphics system disabled.");
    }

    m_sceneManager = std::make_shared<SceneManager>();
}

Engine::~Engine() {
    if (m_renderer) {
        m_renderer->deviceWaitIdle();
        m_renderer.reset();
    }

    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    
    if (!m_config.headless) {
        glfwTerminate();
    }

    m_sceneManager.reset();
    LOG_ENGINE_INFO("Engine shutdown complete, goodbye!");
}

void Engine::initWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(m_config.windowWidth, m_config.windowHeight, m_config.windowTitle, nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

void Engine::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine && engine->m_renderer) {
        engine->m_renderer->setFramebufferResized(true);
    }
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
        if (m_window && !glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        } else if (m_window && glfwWindowShouldClose(m_window)) {
            m_isRunning = false;
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        update(deltaTime);
        
        if (m_renderer && m_window) {
            try {
                m_renderer->drawFrame();
            } catch (const std::exception& e) {
                LOG_ENGINE_ERROR("Render error: " + std::string(e.what()));
                m_isRunning = false;
            }
        } else {
            // In headless mode, we need to sleep a bit to avoid burning 100% CPU
            // as there is no vsync or swapchain waiting.
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }
    }

    if (m_renderer) {
        m_renderer->deviceWaitIdle();
    }
    LOG_ENGINE_INFO("Engine loop stopped.");
}

void Engine::stop() {
    m_isRunning = false;
}

} // namespace vroom
