#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>

class MockWindow {
public:
    MockWindow() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW for tests");
        }
        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // Create a hidden window
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        
        m_window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
        if (!m_window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window for tests");
        }
    }

    ~MockWindow() {
        if (m_window) {
            glfwDestroyWindow(m_window);
        }
        glfwTerminate();
    }

    GLFWwindow* get() const { return m_window; }

private:
    GLFWwindow* m_window = nullptr;
};

