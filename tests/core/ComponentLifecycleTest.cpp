#include <gtest/gtest.h>
#include "vroom/core/Component.hpp"
#include "vroom/core/Entity.hpp"

// Mock Scene for Entity
namespace vroom {
class Scene {};
}

class LifecycleComponent : public vroom::Component {
public:
    int awakeCount = 0;
    int startCount = 0;
    int updateCount = 0;
    int enableCount = 0;
    int disableCount = 0;
    int destroyCount = 0;

    void awake() override { awakeCount++; }
    void start() override { startCount++; }
    void update(float deltaTime) override { updateCount++; }
    void onEnable() override { enableCount++; }
    void onDisable() override { disableCount++; }
    void onDestroy() override { destroyCount++; }
};

TEST(ComponentLifecycleTest, DefaultState) {
    LifecycleComponent comp;
    EXPECT_TRUE(comp.isEnabled());
    EXPECT_FALSE(comp.hasStarted());
    EXPECT_EQ(comp.awakeCount, 0);
    EXPECT_EQ(comp.startCount, 0);
    EXPECT_EQ(comp.updateCount, 0);
    EXPECT_EQ(comp.enableCount, 0);
    EXPECT_EQ(comp.disableCount, 0);
    EXPECT_EQ(comp.destroyCount, 0);
}

TEST(ComponentLifecycleTest, ManualCalls) {
    LifecycleComponent comp;
    
    comp.awake();
    EXPECT_EQ(comp.awakeCount, 1);

    comp.start();
    EXPECT_EQ(comp.startCount, 1);
    
    comp.update(0.16f);
    EXPECT_EQ(comp.updateCount, 1);
    
    comp.onDestroy();
    EXPECT_EQ(comp.destroyCount, 1);
}

TEST(ComponentLifecycleTest, AddComponentCallsAwakeAndOnEnable) {
    vroom::Entity entity(1, nullptr);
    
    auto& comp = entity.addComponent<LifecycleComponent>();
    EXPECT_EQ(comp.awakeCount, 1);
    EXPECT_EQ(comp.enableCount, 1);
    EXPECT_EQ(comp.startCount, 0); 
    EXPECT_TRUE(comp.isEnabled());
}

TEST(ComponentLifecycleTest, EntityUpdateCallsStartAndUpdate) {
    vroom::Entity entity(1, nullptr);
    auto& comp = entity.addComponent<LifecycleComponent>();
    
    // First update: Start + Update
    entity.update(0.1f);
    EXPECT_EQ(comp.startCount, 1);
    EXPECT_EQ(comp.updateCount, 1);
    EXPECT_TRUE(comp.hasStarted());

    // Second update: Update only
    entity.update(0.1f);
    EXPECT_EQ(comp.startCount, 1);
    EXPECT_EQ(comp.updateCount, 2);
}

TEST(ComponentLifecycleTest, EntitySetActiveTriggersCallbacks) {
    vroom::Entity entity(1, nullptr);
    auto& comp = entity.addComponent<LifecycleComponent>();
    // Initial: awake=1, enable=1
    EXPECT_EQ(comp.enableCount, 1);
    EXPECT_EQ(comp.disableCount, 0);

    // Set Entity Inactive -> Calls OnDisable
    entity.setActive(false);
    EXPECT_FALSE(entity.isActive());
    EXPECT_EQ(comp.enableCount, 1);
    EXPECT_EQ(comp.disableCount, 1);

    // Set Entity Active -> Calls OnEnable
    entity.setActive(true);
    EXPECT_TRUE(entity.isActive());
    EXPECT_EQ(comp.enableCount, 2);
    EXPECT_EQ(comp.disableCount, 1);
}

TEST(ComponentLifecycleTest, ComponentSetEnabledChecksEntityActive) {
    vroom::Entity entity(1, nullptr);
    entity.setActive(false);
    
    auto& comp = entity.addComponent<LifecycleComponent>();
    // Entity inactive: awake called, but onEnable skipped
    EXPECT_EQ(comp.awakeCount, 1);
    EXPECT_EQ(comp.enableCount, 0);

    // Component is enabled locally
    EXPECT_TRUE(comp.isEnabled());

    // Toggle component enabled while Entity is inactive
    comp.setEnabled(false);
    EXPECT_EQ(comp.disableCount, 0); // Should NOT call disable, was already effectively disabled
    
    comp.setEnabled(true);
    EXPECT_EQ(comp.enableCount, 0); // Should NOT call enable, entity is inactive

    // Now activate entity -> triggers OnEnable
    entity.setActive(true);
    EXPECT_EQ(comp.enableCount, 1);
}

TEST(ComponentLifecycleTest, EntityDestruction) {
    {
        vroom::Entity entity(1, nullptr);
        auto& comp = entity.addComponent<LifecycleComponent>();
        EXPECT_EQ(comp.destroyCount, 0);
    } // Entity goes out of scope
    // Can't check comp.destroyCount because comp is destroyed.
    // But we can verify logic if we assume the destructor runs.
    // We can't easily test side effects on destroyed objects without external tracking.
}

class ExternalTrackerComponent : public vroom::Component {
public:
    int* destroyCounter;
    ExternalTrackerComponent(int* counter) : destroyCounter(counter) {}
    void onDestroy() override { (*destroyCounter)++; }
};

TEST(ComponentLifecycleTest, EntityDestructionVerified) {
    int destroyCount = 0;
    {
        vroom::Entity entity(1, nullptr);
        entity.addComponent<ExternalTrackerComponent>(&destroyCount);
    }
    EXPECT_EQ(destroyCount, 1);
}

class SelfDisablingComponent : public vroom::Component {
public:
    int updateCount = 0;
    int startCount = 0;

    void start() override {
        startCount++;
        setEnabled(false); // Disable self during start
    }

    void update(float deltaTime) override {
        updateCount++;
    }
};

TEST(ComponentLifecycleTest, DisableInStartPreventsUpdate) {
    vroom::Entity entity(1, nullptr);
    auto& comp = entity.addComponent<SelfDisablingComponent>();
    
    // First update: triggers start(). Component disables itself.
    // If bug exists, update() will still be called this frame.
    entity.update(0.1f);
    
    EXPECT_EQ(comp.startCount, 1);
    EXPECT_EQ(comp.updateCount, 0) << "Update should not be called if disabled in start()";
}
