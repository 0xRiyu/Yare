#ifndef YARE_SCENE_H
#define YARE_SCENE_H

#include <unordered_map>
#include <vector>

#include "Entity.h"

namespace Yare::Graphics {
    class Scene {
       public:
        Scene();
        Entity*                         addEntity(const std::string& entity_name);
        const std::vector<std::string>& getAllEntityNames() const { return m_EntityNames; }
        Entity*                         getEntityByName(const std::string& entity_name);

       private:
        std::vector<std::string>                m_EntityNames;
        std::unordered_map<std::string, Entity> m_Entities;
    };
}  // namespace Yare::Graphics
#endif  // YARE_SCENE_H
