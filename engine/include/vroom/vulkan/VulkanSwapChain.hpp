#pragma once

#include "vroom/vulkan/VulkanDevice.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

struct GLFWwindow;

namespace vroom {

class VulkanSwapChain {
public:
    VulkanSwapChain(VulkanDevice& device, GLFWwindow* window);
    ~VulkanSwapChain();

    // Prevent copying
    VulkanSwapChain(const VulkanSwapChain&) = delete;
    VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;

    void init();
    void recreate();
    void cleanup();

    VkSwapchainKHR getSwapChain() const { return m_swapChain; }
    VkFormat getSwapChainImageFormat() const { return m_swapChainImageFormat; }
    VkExtent2D getSwapChainExtent() const { return m_swapChainExtent; }
    const std::vector<VkImage>& getImages() const { return m_swapChainImages; }
    const std::vector<VkImageView>& getImageViews() const { return m_swapChainImageViews; }
    size_t getImageCount() const { return m_swapChainImages.size(); }
    
    VkResult acquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
    VkResult submitCommandBuffers(const VkCommandBuffer* commandBuffers, uint32_t* imageIndex);

private:
    void createSwapChain();
    void createImageViews();
    
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    VulkanDevice& m_device;
    GLFWwindow* m_window;

    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;
};

} // namespace vroom

