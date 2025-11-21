#pragma once

#include <memory>

namespace vroom {

class Entity;

class Component {
public:
    virtual ~Component() = default;

    Entity* getEntity() const { return m_entity; }
    void setEntity(Entity* entity) { m_entity = entity; }

    // Lifecycle methods
    virtual void awake() {}
    virtual void start() {}
    virtual void update(float deltaTime) {}
    virtual void onEnable() {}
    virtual void onDisable() {}
    virtual void onDestroy() {}

    // State management
    bool isEnabled() const { return m_enabled; }
    bool hasStarted() const { return m_hasStarted; }
    
    /// \brief Sets the enabled state of the component.
    /// This triggers onEnable() or onDisable() if the effective state changes.
    void setEnabled(bool enabled);

    // Internal use for the engine to mark start as called
    void markStarted() { m_hasStarted = true; }

protected:
    Entity* m_entity = nullptr;
    bool m_enabled = true;
    bool m_hasStarted = false;
};

} // namespace vroom
