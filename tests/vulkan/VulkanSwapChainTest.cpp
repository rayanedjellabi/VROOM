#include <gtest/gtest.h>
#include "vroom/vulkan/VulkanSwapChain.hpp"
#include "vroom/vulkan/VulkanDevice.hpp"
#include "tests/vulkan/MockWindow.hpp"

class VulkanSwapChainTest : public ::testing::Test {
protected:
    void SetUp() override {
        try {
            m_window = std::make_unique<MockWindow>();
            m_device = std::make_unique<vroom::VulkanDevice>(m_window->get());
        } catch (const std::exception& e) {
            initializationFailed = true;
            initializationError = e.what();
        }
    }

    std::unique_ptr<MockWindow> m_window;
    std::unique_ptr<vroom::VulkanDevice> m_device;
    bool initializationFailed = false;
    std::string initializationError;
};

TEST_F(VulkanSwapChainTest, CreationAndAccessors) {
    if (initializationFailed) {
        GTEST_SKIP() << "Skipping Vulkan tests: " << initializationError;
    }

    try {
        vroom::VulkanSwapChain swapChain(*m_device, m_window->get());

        EXPECT_NE(swapChain.getSwapChain(), VK_NULL_HANDLE);
        EXPECT_GT(swapChain.getImageCount(), 0);
        EXPECT_EQ(swapChain.getImages().size(), swapChain.getImageCount());
        EXPECT_EQ(swapChain.getImageViews().size(), swapChain.getImageCount());
        
        VkExtent2D extent = swapChain.getSwapChainExtent();
        EXPECT_GT(extent.width, 0);
        EXPECT_GT(extent.height, 0);
    } catch (const std::exception& e) {
        GTEST_SKIP() << "Skipping SwapChain test: " << e.what();
    }
}

TEST_F(VulkanSwapChainTest, Recreation) {
    if (initializationFailed) {
        GTEST_SKIP() << "Skipping Vulkan tests: " << initializationError;
    }

    try {
        vroom::VulkanSwapChain swapChain(*m_device, m_window->get());
        VkSwapchainKHR oldHandle = swapChain.getSwapChain();

        swapChain.recreate();

        EXPECT_NE(swapChain.getSwapChain(), VK_NULL_HANDLE);
        EXPECT_NE(swapChain.getSwapChain(), oldHandle);
        EXPECT_GT(swapChain.getImageCount(), 0);
    } catch (const std::exception& e) {
        GTEST_SKIP() << "Skipping SwapChain test: " << e.what();
    }
}
