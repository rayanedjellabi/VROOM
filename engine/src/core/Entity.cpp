#include "vroom/core/Entity.hpp"

namespace vroom {

Entity::Entity(EntityId id, std::shared_ptr<Scene> scene)
    : m_id(id), m_scene(std::move(scene)) {
}

Entity::~Entity() {
    for (auto& component : m_components) {
        component->onDestroy();
    }
    m_components.clear();
}

void Entity::update(float deltaTime) {
    if (!m_active) {
        return;
    }

    for (auto& component : m_components) {
        if (component->isEnabled()) {
            if (!component->hasStarted()) {
                component->start();
                component->markStarted();
            }
            
            // Check enabled again in case start() disabled the component
            if (component->isEnabled()) {
                component->update(deltaTime);
            }
        }
    }
}

void Entity::setActive(bool active) {
    if (m_active == active) {
        return;
    }

    m_active = active;

    for (auto& component : m_components) {
        if (component->isEnabled()) {
            if (m_active) {
                component->onEnable();
            } else {
                component->onDisable();
            }
        }
    }
}

} // namespace vroom
