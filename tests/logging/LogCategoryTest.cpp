#include <gtest/gtest.h>
#include "vroom/logging/LogCategory.hpp"

using namespace vroom::logging;

class LogCategoryTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

// Test LogCategoryToString for Engine
TEST_F(LogCategoryTest, EngineToString) {
    EXPECT_STREQ(LogCategoryToString(LogCategory::Engine), "VROOM");
}

// Test LogCategoryToString for Application
TEST_F(LogCategoryTest, ApplicationToString) {
    EXPECT_STREQ(LogCategoryToString(LogCategory::Application), "APP");
}

// Test that all enum values are handled
TEST_F(LogCategoryTest, AllCategoriesHandled) {
    EXPECT_STREQ(LogCategoryToString(LogCategory::Engine), "VROOM");
    EXPECT_STREQ(LogCategoryToString(LogCategory::Application), "APP");
}

// Test that the function returns non-null for all valid categories
TEST_F(LogCategoryTest, NonNullForValidCategories) {
    EXPECT_NE(LogCategoryToString(LogCategory::Engine), nullptr);
    EXPECT_NE(LogCategoryToString(LogCategory::Application), nullptr);
}

// Test that different categories return different strings
TEST_F(LogCategoryTest, DifferentCategoriesDifferentStrings) {
    EXPECT_STRNE(LogCategoryToString(LogCategory::Engine), 
                 LogCategoryToString(LogCategory::Application));
}

