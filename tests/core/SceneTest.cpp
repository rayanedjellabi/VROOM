#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "vroom/core/Scene.hpp"
#include "vroom/core/Component.hpp"

using namespace vroom;

class SceneTest : public ::testing::Test {
protected:
    void SetUp() override {
        scene = std::make_shared<Scene>();
    }

    std::shared_ptr<Scene> scene;
};

class MockComponent : public Component {
public:
    MOCK_METHOD(void, update, (float), (override));
    MOCK_METHOD(void, onEnable, (), (override));
    MOCK_METHOD(void, onDisable, (), (override));
    MOCK_METHOD(void, onDestroy, (), (override));
};

TEST_F(SceneTest, CreateEntity) {
    Entity& entity = scene->createEntity();
    EXPECT_TRUE(entity.isActive());
    EXPECT_EQ(scene->getRootEntities().size(), 1);
    EXPECT_EQ(scene->getRootEntities()[0], &entity);
}

TEST_F(SceneTest, CreateMultipleEntities) {
    Entity& entity1 = scene->createEntity();
    Entity& entity2 = scene->createEntity();
    
    EXPECT_EQ(scene->getRootEntities().size(), 2);
    EXPECT_NE(&entity1, &entity2);
}

TEST_F(SceneTest, DestroyEntity) {
    Entity& entity = scene->createEntity();
    scene->destroyEntity(entity);
    
    EXPECT_EQ(scene->getRootEntities().size(), 0);
}

TEST_F(SceneTest, DestroyEntityWithChildren) {
    Entity& parent = scene->createEntity();
    Entity& child = scene->createEntity();
    
    parent.addChild(&child);
    
    EXPECT_EQ(scene->getRootEntities().size(), 1);
    EXPECT_EQ(parent.getChildren().size(), 1);
    
    scene->destroyEntity(parent);
    
    EXPECT_EQ(scene->getRootEntities().size(), 0);
}

TEST_F(SceneTest, ClearScene) {
    scene->createEntity();
    scene->createEntity();
    scene->createEntity();
    
    EXPECT_EQ(scene->getRootEntities().size(), 3);
    
    scene->clear();
    
    EXPECT_EQ(scene->getRootEntities().size(), 0);
}

TEST_F(SceneTest, UpdateEntities) {
    Entity& entity = scene->createEntity();
    // We use unique_ptr directly for the mock to control lifetime or let scene handle it.
    // But entity.addComponent creates the component internally. 
    // Since MockComponent is a strict mock by default if using MOCK_METHOD, 
    // we need to ensure we don't leak expectations.
    // However, Entity manages the lifetime. The issue is likely that Google Mock checks expectations 
    // in the destructor, and if the entity is still alive when test ends (which it is, inside 'scene'),
    // the component is still alive.
    // The scene is member variable 'scene', which is destroyed when SceneTest is destroyed (after the test body).
    // BUT, Google Mock verifies expectations at the end of the test body for local objects, 
    // or when the mock is destroyed. 
    // If the mock is destroyed AFTER the test body (in SetUp/TearDown or destructor of fixture), 
    // gtest might complain if it thinks it leaked.
    
    // Actually, the error "should be deleted but never is" means the mock object is not destroyed before the verification 
    // phase or program exit. 
    // 'scene' is a shared_ptr in the fixture. It is destroyed when the fixture is destroyed.
    // The fixture is destroyed after each test.
    
    // Let's try explicit destruction of the entity or clearing the scene at the end of the test.
    
    auto& component = entity.addComponent<MockComponent>();
    EXPECT_CALL(component, update(0.1f)).Times(1);
    
    scene->update(0.1f);
    
    // Clear scene to destroy components and verify expectations
    scene->clear();
}

TEST_F(SceneTest, UpdateOnlyActiveEntities) {
    Entity& entity = scene->createEntity();
    auto& component = entity.addComponent<MockComponent>();
    
    entity.setActive(false);
    
    EXPECT_CALL(component, update(testing::_)).Times(0);
    
    scene->update(0.1f);
    
    scene->clear();
}

TEST_F(SceneTest, GetRootEntitiesOnlyReturnsRoots) {
    Entity& parent = scene->createEntity();
    Entity& child = scene->createEntity();
    
    parent.addChild(&child);
    
    auto roots = scene->getRootEntities();
    EXPECT_EQ(roots.size(), 1);
    EXPECT_EQ(roots[0], &parent);
}

