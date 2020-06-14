#ifndef YARE_SCENE_H
#define YARE_SCENE_H

#include "Graphics/MeshInstance.h"
#include <vector>

namespace Yare::Graphics {

    class Scene {
    public:
        Scene();

        const std::vector<MeshInstance>& getMeshInstances() const { return m_MeshInstances; }

    private:
        std::vector<MeshInstance> m_MeshInstances;

    };
}
#endif //YARE_SCENE_H
