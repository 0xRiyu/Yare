#ifndef YAREZO_SCENE_H
#define YAREZO_SCENE_H

#include "Graphics/MeshInstance.h"

#include <vector>


namespace Yarezo::Graphics {

    class Scene {
    public:
        Scene();

        const std::vector<MeshInstance>& getMeshInstances() const { return m_MeshInstances; }

    private:
        std::vector<MeshInstance> m_MeshInstances;

    };
}
#endif //YAREZO_SCENE_H
