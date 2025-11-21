#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "vroom/core/Engine.hpp"
#include "vroom/core/Component.hpp"
#include <thread>
#include <chrono>

using namespace vroom;

// A simple component to track updates
class UpdateTrackerComponent : public Component {
public:
    void update(float deltaTime) override {
        updateCount++;
        totalDeltaTime += deltaTime;
    }

    int updateCount = 0;
    float totalDeltaTime = 0.0f;
};

class EngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        engine = std::make_unique<Engine>();
    }

    void TearDown() override {
        engine.reset();
    }

    std::unique_ptr<Engine> engine;
};

TEST_F(EngineTest, UpdatePassesToSceneManager) {
    // Setup a scene with a tracking component
    auto& sceneManager = engine->getSceneManager();
    auto scene = std::make_shared<Scene>();
    auto& entity = scene->createEntity();
    auto& tracker = entity.addComponent<UpdateTrackerComponent>();
    
    // Manually inject scene (since we can't easily mock createSceneFromFile in Engine's SceneManager)
    // Wait, we need to load the scene. SceneManager::loadScene calls createSceneFromFile.
    // But SceneManager also has internal storage. 
    // The only way to inject a pre-made scene without mocking createSceneFromFile is if there was a method for it.
    // Engine creates a default SceneManager, not a mockable one.
    
    // However, we can use the fact that SceneManager starts with an empty scene or we can just use what we have.
    // SceneManager has m_activeScene. But there is no setActiveScene public method.
    // But loadScene calls createSceneFromFile.
    
    // Let's rely on the fact that SceneManagerTest showed we can mock SceneManager if we could inject it.
    // But Engine doesn't allow injection.
    
    // Workaround: Use the default SceneManager but manipulate it.
    // SceneManager interface: loadScene(path).
    // We can't easily inject a scene object unless we subclass SceneManager and Engine allows using it.
    
    // Wait, SceneManager DOES NOT have a method to add an existing scene object directly.
    // It only has loadScene(string path) which calls createSceneFromFile(path).
    
    // BUT, createSceneFromFile is protected.
    // And SceneManager is created inside Engine.
    
    // Let's look at Engine.hpp again.
    // Engine has getSceneManager().
    
    // Can we get the active scene and add an entity to it?
    // SceneManager starts with... let's check SceneManager constructor.
    // It initializes m_activeScene = std::make_shared<Scene>();
    
    auto activeScene = sceneManager.getActiveScene();
    ASSERT_NE(activeScene, nullptr);
    
    auto& entityInDefault = activeScene->createEntity();
    auto& component = entityInDefault.addComponent<UpdateTrackerComponent>();
    
    engine->update(0.1f);
    
    EXPECT_EQ(component.updateCount, 1);
    EXPECT_FLOAT_EQ(component.totalDeltaTime, 0.1f);
}

TEST_F(EngineTest, RunLoopUpdatesComponents) {
    auto activeScene = engine->getSceneManager().getActiveScene();
    ASSERT_NE(activeScene, nullptr);
    
    auto& entity = activeScene->createEntity();
    auto& component = entity.addComponent<UpdateTrackerComponent>();
    
    // Run engine in a separate thread
    std::thread engineThread([this]() {
        engine->run();
    });
    
    // Let it run for a bit
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Stop the engine
    engine->stop();
    
    // Wait for thread to finish
    engineThread.join();
    
    // Verify updates happened
    EXPECT_GT(component.updateCount, 0);
    EXPECT_GT(component.totalDeltaTime, 0.0f);
}

TEST_F(EngineTest, StopTerminatesLoop) {
    // This test effectively duplicates the logic of RunLoopUpdatesComponents
    // but focuses on the fact that join() returns, meaning the loop exited.
    
    std::thread engineThread([this]() {
        engine->run();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    engine->stop();
    
    // If stop doesn't work, this will hang (or we could use a future with timeout)
    engineThread.join();
    
    // If we reached here, stop() successfully broke the loop
    SUCCEED();
}

