#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <string>

#include "vroom/vulkan/VulkanDevice.hpp"
#include "vroom/vulkan/VulkanSwapChain.hpp"

struct GLFWwindow;

namespace vroom {

class VulkanRenderer {
public:
    VulkanRenderer();
    ~VulkanRenderer();

    void init(GLFWwindow* window);
    void drawFrame();
    void deviceWaitIdle();
    void setFramebufferResized(bool resized) { m_framebufferResized = resized; }

private:
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandBuffers();
    void createSyncObjects();
    
    void recreateSwapChain();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    VkShaderModule createShaderModule(const std::vector<char>& code);
    static std::vector<char> readFile(const std::string& filename);

    GLFWwindow* m_window = nullptr;
    
    std::unique_ptr<VulkanDevice> m_device;
    std::unique_ptr<VulkanSwapChain> m_swapChain;

    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;

    std::vector<VkFramebuffer> m_swapChainFramebuffers;
    std::vector<VkCommandBuffer> m_commandBuffers;

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;

    uint32_t m_currentFrame = 0;
    bool m_framebufferResized = false;

    const int MAX_FRAMES_IN_FLIGHT = 2;
};

} // namespace vroom
