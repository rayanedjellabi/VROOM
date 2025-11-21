#include <gtest/gtest.h>
#include "vroom/core/Entity.hpp"
#include "vroom/core/Component.hpp"

// Mock Scene for Entity
namespace vroom {
class Scene {};
}

class HierarchyTrackerComponent : public vroom::Component {
public:
    int enableCount = 0;
    int disableCount = 0;
    int updateCount = 0;

    void onEnable() override { enableCount++; }
    void onDisable() override { disableCount++; }
    void update(float dt) override { updateCount++; }
};

TEST(EntityHierarchyTest, AddRemoveChild) {
    vroom::Entity parent(1, nullptr);
    vroom::Entity child(2, nullptr);

    parent.addChild(&child);
    EXPECT_EQ(child.getParent(), &parent);
    EXPECT_EQ(parent.getChildren().size(), 1);
    EXPECT_EQ(parent.getChildren()[0], &child);

    parent.removeChild(&child);
    EXPECT_EQ(child.getParent(), nullptr);
    EXPECT_TRUE(parent.getChildren().empty());
}

TEST(EntityHierarchyTest, SetParent) {
    vroom::Entity parent(1, nullptr);
    vroom::Entity child(2, nullptr);

    child.setParent(&parent);
    EXPECT_EQ(child.getParent(), &parent);
    EXPECT_EQ(parent.getChildren().size(), 1);

    child.setParent(nullptr);
    EXPECT_EQ(child.getParent(), nullptr);
    EXPECT_TRUE(parent.getChildren().empty());
}

TEST(EntityHierarchyTest, ActiveStatePropagation) {
    vroom::Entity parent(1, nullptr);
    vroom::Entity child(2, nullptr);
    
    parent.addChild(&child);
    auto& comp = child.addComponent<HierarchyTrackerComponent>();
    
    // Initial state: both active
    EXPECT_TRUE(parent.isActive());
    EXPECT_TRUE(child.isActive());
    EXPECT_EQ(comp.enableCount, 1); // enabled on creation

    // Disable parent -> child should be inactive
    parent.setActive(false);
    EXPECT_FALSE(parent.isActive());
    EXPECT_FALSE(child.isActive());
    EXPECT_EQ(comp.disableCount, 1);

    // Enable parent -> child should be active
    parent.setActive(true);
    EXPECT_TRUE(child.isActive());
    EXPECT_EQ(comp.enableCount, 2);
}

TEST(EntityHierarchyTest, ChildLocalState) {
    vroom::Entity parent(1, nullptr);
    vroom::Entity child(2, nullptr);
    
    parent.addChild(&child);
    auto& comp = child.addComponent<HierarchyTrackerComponent>();

    // Disable child locally
    child.setActive(false);
    EXPECT_FALSE(child.isActive());
    EXPECT_EQ(comp.disableCount, 1);

    // Disable parent -> no change for child (already inactive)
    parent.setActive(false);
    EXPECT_FALSE(child.isActive());
    EXPECT_EQ(comp.disableCount, 1); // Still 1

    // Enable parent -> child still inactive (locally false)
    parent.setActive(true);
    EXPECT_FALSE(child.isActive());
    EXPECT_EQ(comp.enableCount, 1); // Still 1 (initial)

    // Enable child
    child.setActive(true);
    EXPECT_TRUE(child.isActive());
    EXPECT_EQ(comp.enableCount, 2);
}

TEST(EntityHierarchyTest, DeepHierarchy) {
    vroom::Entity root(1, nullptr);
    vroom::Entity mid(2, nullptr);
    vroom::Entity leaf(3, nullptr);

    root.addChild(&mid);
    mid.addChild(&leaf);

    auto& comp = leaf.addComponent<HierarchyTrackerComponent>();

    // Disable root
    root.setActive(false);
    EXPECT_FALSE(leaf.isActive());
    EXPECT_EQ(comp.disableCount, 1);

    // Enable root
    root.setActive(true);
    EXPECT_TRUE(leaf.isActive());
    EXPECT_EQ(comp.enableCount, 2);
}

TEST(EntityHierarchyTest, UpdatePropagation) {
    vroom::Entity parent(1, nullptr);
    vroom::Entity child(2, nullptr);
    
    parent.addChild(&child);
    auto& pComp = parent.addComponent<HierarchyTrackerComponent>();
    auto& cComp = child.addComponent<HierarchyTrackerComponent>();

    parent.update(0.1f);
    EXPECT_EQ(pComp.updateCount, 1);
    EXPECT_EQ(cComp.updateCount, 1);

    // Disable child
    child.setActive(false);
    parent.update(0.1f);
    EXPECT_EQ(pComp.updateCount, 2);
    EXPECT_EQ(cComp.updateCount, 1); // Skipped

    // Disable parent
    child.setActive(true);
    parent.setActive(false);
    parent.update(0.1f);
    EXPECT_EQ(pComp.updateCount, 2); // Skipped
    EXPECT_EQ(cComp.updateCount, 1); // Skipped
}

TEST(EntityHierarchyTest, Reparenting) {
    vroom::Entity parent1(1, nullptr);
    vroom::Entity parent2(2, nullptr);
    vroom::Entity child(3, nullptr);
    auto& comp = child.addComponent<HierarchyTrackerComponent>();

    parent1.addChild(&child);
    
    parent1.setActive(false);
    // Child becomes inactive
    EXPECT_FALSE(child.isActive());
    EXPECT_EQ(comp.disableCount, 1);

    // Move to active parent2
    parent2.addChild(&child);
    EXPECT_TRUE(child.isActive());
    EXPECT_EQ(comp.enableCount, 2);
    
    EXPECT_EQ(parent1.getChildren().size(), 0);
    EXPECT_EQ(parent2.getChildren().size(), 1);
}

TEST(EntityHierarchyTest, CyclePrevention) {
    vroom::Entity p(1, nullptr);
    vroom::Entity c(2, nullptr);
    
    p.addChild(&c);
    // Try to make p a child of c (cycle)
    c.addChild(&p);
    
    EXPECT_EQ(p.getParent(), nullptr);
    EXPECT_EQ(c.getParent(), &p);
    
    // Self parenting
    p.setParent(&p);
    EXPECT_EQ(p.getParent(), nullptr);
}

