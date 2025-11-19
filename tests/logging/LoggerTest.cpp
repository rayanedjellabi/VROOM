#include <gtest/gtest.h>
#include "vroom/logging/Logger.hpp"
#include "vroom/logging/LogLevel.hpp"
#include "vroom/logging/LogCategory.hpp"
#include <sstream>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace vroom::logging;

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset streams to defaults before each test
        logger.ResetEngineStream();
        logger.ResetApplicationStream();
        logger.ResetEngineErrorStream();
        logger.ResetApplicationErrorStream();
        
        // Clear test streams
        testStream.str("");
        testStream.clear();
        testErrorStream.str("");
        testErrorStream.clear();
    }

    Logger& logger = Logger::GetInstance();
    std::ostringstream testStream;
    std::ostringstream testErrorStream;
};

// Test singleton pattern
TEST_F(LoggerTest, SingletonInstance) {
    Logger& instance1 = Logger::GetInstance();
    Logger& instance2 = Logger::GetInstance();
    
    EXPECT_EQ(&instance1, &instance2);
}

// Test default stream initialization
TEST_F(LoggerTest, DefaultStreams) {
    // Default streams should be std::cout and std::cerr
    // We can't easily test this without capturing stdout/stderr,
    // but we can test that setting and resetting works
    logger.SetEngineStream(&testStream);
    logger.ResetEngineStream();
    
    // After reset, should be back to default (std::cout)
    // We'll verify this by setting it again and checking it's not our test stream
    std::ostringstream anotherStream;
    logger.SetEngineStream(&anotherStream);
    logger.Log(LogLevel::Info, LogCategory::Engine, "TestClass", "test");
    
    // The message should go to anotherStream, not testStream
    EXPECT_TRUE(anotherStream.str().find("test") != std::string::npos);
    EXPECT_TRUE(testStream.str().empty());
}

// Test SetEngineStream
TEST_F(LoggerTest, SetEngineStream) {
    logger.SetEngineStream(&testStream);
    logger.Log(LogLevel::Info, LogCategory::Engine, "TestClass", "test message");
    
    EXPECT_TRUE(testStream.str().find("test message") != std::string::npos);
    EXPECT_TRUE(testStream.str().find("[VROOM]") != std::string::npos);
    EXPECT_TRUE(testStream.str().find("[INFO]") != std::string::npos);
    EXPECT_TRUE(testStream.str().find("[TestClass]") != std::string::npos);
}

// Test SetApplicationStream
TEST_F(LoggerTest, SetApplicationStream) {
    logger.SetApplicationStream(&testStream);
    logger.Log(LogLevel::Info, LogCategory::Application, "TestClass", "app message");
    
    EXPECT_TRUE(testStream.str().find("app message") != std::string::npos);
    EXPECT_TRUE(testStream.str().find("[APP]") != std::string::npos);
    EXPECT_TRUE(testStream.str().find("[INFO]") != std::string::npos);
}

// Test SetEngineErrorStream
TEST_F(LoggerTest, SetEngineErrorStream) {
    logger.SetEngineErrorStream(&testErrorStream);
    logger.Log(LogLevel::Error, LogCategory::Engine, "TestClass", "error message");
    
    EXPECT_TRUE(testErrorStream.str().find("error message") != std::string::npos);
    EXPECT_TRUE(testErrorStream.str().find("[VROOM]") != std::string::npos);
    EXPECT_TRUE(testErrorStream.str().find("[ERROR]") != std::string::npos);
}

// Test SetApplicationErrorStream
TEST_F(LoggerTest, SetApplicationErrorStream) {
    logger.SetApplicationErrorStream(&testErrorStream);
    logger.Log(LogLevel::Error, LogCategory::Application, "TestClass", "app error");
    
    EXPECT_TRUE(testErrorStream.str().find("app error") != std::string::npos);
    EXPECT_TRUE(testErrorStream.str().find("[APP]") != std::string::npos);
    EXPECT_TRUE(testErrorStream.str().find("[ERROR]") != std::string::npos);
}

// Test that Error level uses error streams
TEST_F(LoggerTest, ErrorLevelUsesErrorStream) {
    logger.SetEngineStream(&testStream);
    logger.SetEngineErrorStream(&testErrorStream);
    
    logger.Log(LogLevel::Error, LogCategory::Engine, "TestClass", "error");
    
    EXPECT_TRUE(testErrorStream.str().find("error") != std::string::npos);
    EXPECT_TRUE(testStream.str().empty());
}

// Test that non-Error levels use normal streams
TEST_F(LoggerTest, NonErrorLevelsUseNormalStream) {
    logger.SetEngineStream(&testStream);
    logger.SetEngineErrorStream(&testErrorStream);
    
    logger.Log(LogLevel::Info, LogCategory::Engine, "TestClass", "info");
    logger.Log(LogLevel::Warning, LogCategory::Engine, "TestClass", "warning");
    logger.Log(LogLevel::Debug, LogCategory::Engine, "TestClass", "debug");
    
    EXPECT_TRUE(testStream.str().find("info") != std::string::npos);
    EXPECT_TRUE(testStream.str().find("warning") != std::string::npos);
    EXPECT_TRUE(testStream.str().find("debug") != std::string::npos);
    EXPECT_TRUE(testErrorStream.str().empty());
}

// Test null stream handling (should default to std::cout/std::cerr)
TEST_F(LoggerTest, NullStreamHandling) {
    logger.SetEngineStream(nullptr);
    logger.SetApplicationStream(nullptr);
    logger.SetEngineErrorStream(nullptr);
    logger.SetApplicationErrorStream(nullptr);
    
    // Should not crash and should use defaults
    EXPECT_NO_THROW({
        logger.Log(LogLevel::Info, LogCategory::Engine, "TestClass", "test");
        logger.Log(LogLevel::Error, LogCategory::Engine, "TestClass", "error");
    });
}

// Test ResetEngineStream
TEST_F(LoggerTest, ResetEngineStream) {
    logger.SetEngineStream(&testStream);
    logger.ResetEngineStream();
    
    std::ostringstream newStream;
    logger.SetEngineStream(&newStream);
    logger.Log(LogLevel::Info, LogCategory::Engine, "TestClass", "test");
    
    EXPECT_TRUE(newStream.str().find("test") != std::string::npos);
    EXPECT_TRUE(testStream.str().empty());
}

// Test ResetApplicationStream
TEST_F(LoggerTest, ResetApplicationStream) {
    logger.SetApplicationStream(&testStream);
    logger.ResetApplicationStream();
    
    std::ostringstream newStream;
    logger.SetApplicationStream(&newStream);
    logger.Log(LogLevel::Info, LogCategory::Application, "TestClass", "test");
    
    EXPECT_TRUE(newStream.str().find("test") != std::string::npos);
    EXPECT_TRUE(testStream.str().empty());
}

// Test ResetEngineErrorStream
TEST_F(LoggerTest, ResetEngineErrorStream) {
    logger.SetEngineErrorStream(&testErrorStream);
    logger.ResetEngineErrorStream();
    
    std::ostringstream newStream;
    logger.SetEngineErrorStream(&newStream);
    logger.Log(LogLevel::Error, LogCategory::Engine, "TestClass", "test");
    
    EXPECT_TRUE(newStream.str().find("test") != std::string::npos);
    EXPECT_TRUE(testErrorStream.str().empty());
}

// Test ResetApplicationErrorStream
TEST_F(LoggerTest, ResetApplicationErrorStream) {
    logger.SetApplicationErrorStream(&testErrorStream);
    logger.ResetApplicationErrorStream();
    
    std::ostringstream newStream;
    logger.SetApplicationErrorStream(&newStream);
    logger.Log(LogLevel::Error, LogCategory::Application, "TestClass", "test");
    
    EXPECT_TRUE(newStream.str().find("test") != std::string::npos);
    EXPECT_TRUE(testErrorStream.str().empty());
}

// Test Debug convenience method
TEST_F(LoggerTest, DebugMethod) {
    logger.SetEngineStream(&testStream);
    logger.Debug(LogCategory::Engine, "TestClass", "debug message");
    
    EXPECT_TRUE(testStream.str().find("debug message") != std::string::npos);
    EXPECT_TRUE(testStream.str().find("[DEBUG]") != std::string::npos);
}

// Test Info convenience method
TEST_F(LoggerTest, InfoMethod) {
    logger.SetEngineStream(&testStream);
    logger.Info(LogCategory::Engine, "TestClass", "info message");
    
    EXPECT_TRUE(testStream.str().find("info message") != std::string::npos);
    EXPECT_TRUE(testStream.str().find("[INFO]") != std::string::npos);
}

// Test Warning convenience method
TEST_F(LoggerTest, WarningMethod) {
    logger.SetEngineStream(&testStream);
    logger.Warning(LogCategory::Engine, "TestClass", "warning message");
    
    EXPECT_TRUE(testStream.str().find("warning message") != std::string::npos);
    EXPECT_TRUE(testStream.str().find("[WARNING]") != std::string::npos);
}

// Test Error convenience method
TEST_F(LoggerTest, ErrorMethod) {
    logger.SetEngineErrorStream(&testErrorStream);
    logger.Error(LogCategory::Engine, "TestClass", "error message");
    
    EXPECT_TRUE(testErrorStream.str().find("error message") != std::string::npos);
    EXPECT_TRUE(testErrorStream.str().find("[ERROR]") != std::string::npos);
}

// Test message formatting includes timestamp
TEST_F(LoggerTest, MessageFormattingIncludesTimestamp) {
    logger.SetEngineStream(&testStream);
    logger.Log(LogLevel::Info, LogCategory::Engine, "TestClass", "test");
    
    std::string output = testStream.str();
    // Check for timestamp format [HH:MM:SS.mmm]
    EXPECT_TRUE(output.find("[") != std::string::npos);
    EXPECT_TRUE(output.find(":") != std::string::npos);
    EXPECT_TRUE(output.find(".") != std::string::npos);
}

// Test message formatting includes all components
TEST_F(LoggerTest, MessageFormattingComponents) {
    logger.SetApplicationStream(&testStream);
    logger.Log(LogLevel::Warning, LogCategory::Application, "MyClass", "my message");
    
    std::string output = testStream.str();
    EXPECT_TRUE(output.find("[APP]") != std::string::npos);
    EXPECT_TRUE(output.find("[WARNING]") != std::string::npos);
    EXPECT_TRUE(output.find("[MyClass]") != std::string::npos);
    EXPECT_TRUE(output.find("my message") != std::string::npos);
}

// Test thread safety
TEST_F(LoggerTest, ThreadSafety) {
    logger.SetEngineStream(&testStream);
    
    const int numThreads = 10;
    const int messagesPerThread = 100;
    std::vector<std::thread> threads;
    
    // Get reference to logger instance for lambda capture
    Logger& loggerRef = Logger::GetInstance();
    
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&loggerRef, i]() {
            for (int j = 0; j < messagesPerThread; ++j) {
                loggerRef.Log(LogLevel::Info, LogCategory::Engine, 
                          "Thread" + std::to_string(i), 
                          "message" + std::to_string(j));
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Check that all messages were logged (should have numThreads * messagesPerThread lines)
    std::string output = testStream.str();
    size_t newlineCount = std::count(output.begin(), output.end(), '\n');
    EXPECT_EQ(newlineCount, numThreads * messagesPerThread);
}

// Test that messages are flushed
TEST_F(LoggerTest, MessageFlushing) {
    logger.SetEngineStream(&testStream);
    logger.Log(LogLevel::Info, LogCategory::Engine, "TestClass", "test");
    
    // Stream should be flushed, so we can immediately read it
    EXPECT_FALSE(testStream.str().empty());
}

// Test empty message
TEST_F(LoggerTest, EmptyMessage) {
    logger.SetEngineStream(&testStream);
    logger.Log(LogLevel::Info, LogCategory::Engine, "TestClass", "");
    
    std::string output = testStream.str();
    EXPECT_TRUE(output.find("[TestClass]") != std::string::npos);
    // Should end with just a newline after the class name
    EXPECT_TRUE(output.find("\n") != std::string::npos);
}

// Test empty class name
TEST_F(LoggerTest, EmptyClassName) {
    logger.SetEngineStream(&testStream);
    logger.Log(LogLevel::Info, LogCategory::Engine, "", "test message");
    
    std::string output = testStream.str();
    EXPECT_TRUE(output.find("test message") != std::string::npos);
    EXPECT_TRUE(output.find("[]") != std::string::npos);
}

// Test multiple log calls
TEST_F(LoggerTest, MultipleLogCalls) {
    logger.SetEngineStream(&testStream);
    logger.SetApplicationStream(&testStream);
    
    logger.Log(LogLevel::Debug, LogCategory::Engine, "Class1", "message1");
    logger.Log(LogLevel::Info, LogCategory::Application, "Class2", "message2");
    logger.Log(LogLevel::Warning, LogCategory::Engine, "Class3", "message3");
    
    std::string output = testStream.str();
    EXPECT_TRUE(output.find("message1") != std::string::npos);
    EXPECT_TRUE(output.find("message2") != std::string::npos);
    EXPECT_TRUE(output.find("message3") != std::string::npos);
}

