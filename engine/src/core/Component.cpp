#include "vroom/core/Component.hpp"
#include "vroom/core/Entity.hpp"

namespace vroom {

void Component::setEnabled(bool enabled) {
    if (m_enabled != enabled) {
        m_enabled = enabled;
        
        // Only trigger callbacks if the Entity allows it (active or null)
        // In Unity, if GameObject is inactive, enabling a component doesn't trigger OnEnable.
        // But if GameObject is active, it does.
        
        bool isEntityActive = (!m_entity || m_entity->isActive());
        
        if (isEntityActive) {
            if (m_enabled) {
                onEnable();
            } else {
                onDisable();
            }
        }
    }
}

SceneManager* Component::getSceneManager() const {
    if (m_entity) {
        return m_entity->getSceneManager();
    }
    return nullptr;
}

} // namespace vroom

