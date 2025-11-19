#include <gtest/gtest.h>
#include "vroom/logging/LogLevel.hpp"

using namespace vroom::logging;

class LogLevelTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

// Test LogLevelToString for Debug
TEST_F(LogLevelTest, DebugToString) {
    EXPECT_STREQ(LogLevelToString(LogLevel::Debug), "DEBUG");
}

// Test LogLevelToString for Info
TEST_F(LogLevelTest, InfoToString) {
    EXPECT_STREQ(LogLevelToString(LogLevel::Info), "INFO");
}

// Test LogLevelToString for Warning
TEST_F(LogLevelTest, WarningToString) {
    EXPECT_STREQ(LogLevelToString(LogLevel::Warning), "WARNING");
}

// Test LogLevelToString for Error
TEST_F(LogLevelTest, ErrorToString) {
    EXPECT_STREQ(LogLevelToString(LogLevel::Error), "ERROR");
}

// Test that all enum values are handled
TEST_F(LogLevelTest, AllLevelsHandled) {
    EXPECT_STREQ(LogLevelToString(LogLevel::Debug), "DEBUG");
    EXPECT_STREQ(LogLevelToString(LogLevel::Info), "INFO");
    EXPECT_STREQ(LogLevelToString(LogLevel::Warning), "WARNING");
    EXPECT_STREQ(LogLevelToString(LogLevel::Error), "ERROR");
}

// Test that the function returns non-null for all valid levels
TEST_F(LogLevelTest, NonNullForValidLevels) {
    EXPECT_NE(LogLevelToString(LogLevel::Debug), nullptr);
    EXPECT_NE(LogLevelToString(LogLevel::Info), nullptr);
    EXPECT_NE(LogLevelToString(LogLevel::Warning), nullptr);
    EXPECT_NE(LogLevelToString(LogLevel::Error), nullptr);
}

// Test that different levels return different strings
TEST_F(LogLevelTest, DifferentLevelsDifferentStrings) {
    EXPECT_STRNE(LogLevelToString(LogLevel::Debug), LogLevelToString(LogLevel::Info));
    EXPECT_STRNE(LogLevelToString(LogLevel::Info), LogLevelToString(LogLevel::Warning));
    EXPECT_STRNE(LogLevelToString(LogLevel::Warning), LogLevelToString(LogLevel::Error));
    EXPECT_STRNE(LogLevelToString(LogLevel::Debug), LogLevelToString(LogLevel::Error));
}

