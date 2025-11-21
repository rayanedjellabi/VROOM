#include <gtest/gtest.h>
#include "vroom/vulkan/VulkanDevice.hpp"
#include "tests/vulkan/MockWindow.hpp"

class VulkanDeviceTest : public ::testing::Test {
protected:
    void SetUp() override {
        try {
            m_window = std::make_unique<MockWindow>();
        } catch (const std::exception& e) {
            // If window creation fails (headless?), we can't test device
            initializationFailed = true;
            initializationError = e.what();
        }
    }

    std::unique_ptr<MockWindow> m_window;
    bool initializationFailed = false;
    std::string initializationError;
};

TEST_F(VulkanDeviceTest, Initialization) {
    if (initializationFailed) {
        GTEST_SKIP() << "Skipping Vulkan tests due to window creation failure: " << initializationError;
    }

    try {
        vroom::VulkanDevice device(m_window->get());
        
        EXPECT_NE(device.getInstance(), VK_NULL_HANDLE);
        EXPECT_NE(device.getPhysicalDevice(), VK_NULL_HANDLE);
        EXPECT_NE(device.getDevice(), VK_NULL_HANDLE);
        EXPECT_NE(device.getSurface(), VK_NULL_HANDLE);
        EXPECT_NE(device.getGraphicsQueue(), VK_NULL_HANDLE);
        EXPECT_NE(device.getPresentQueue(), VK_NULL_HANDLE);
        EXPECT_NE(device.getCommandPool(), VK_NULL_HANDLE);

        auto indices = device.findQueueFamilies(device.getPhysicalDevice());
        EXPECT_TRUE(indices.isComplete());

    } catch (const std::runtime_error& e) {
        GTEST_SKIP() << "Skipping Vulkan tests due to device initialization failure (no GPU?): " << e.what();
    }
}

TEST_F(VulkanDeviceTest, SwapChainSupport) {
    if (initializationFailed) {
        GTEST_SKIP() << "Skipping Vulkan tests due to window creation failure: " << initializationError;
    }

    try {
        vroom::VulkanDevice device(m_window->get());
        auto details = device.querySwapChainSupport(device.getPhysicalDevice());
        EXPECT_FALSE(details.formats.empty());
        EXPECT_FALSE(details.presentModes.empty());
    } catch (const std::runtime_error& e) {
        GTEST_SKIP() << "Skipping Vulkan tests due to device initialization failure: " << e.what();
    }
}
