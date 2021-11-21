#include "Graphics/Scene/Scene.h"

namespace Yare::Graphics {

    Scene::Scene() {}

    Entity* Scene::addEntity(const std::string& entity_name) {
        m_EntityNames.push_back(entity_name);
        // Create the key in the map
        return &m_Entities[entity_name];
    }

    Entity* Scene::getEntityByName(const std::string& entity_name) {
        auto loc = m_Entities.find(entity_name);
        if (loc != m_Entities.end()) {
            return &loc->second;
        }
        return nullptr;
    }
}  // namespace Yare::Graphics
