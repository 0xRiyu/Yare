#ifndef YARE_MESH_INSTANCE_H
#define YARE_MESH_INSTANCE_H

#include "Graphics/Mesh.h"
#include "Graphics/Material.h"

#include "Core/Transform.h"

#include <string>
#include <vector>

namespace Yare::Graphics {

    class MeshInstance {
    public:
        MeshInstance(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
        MeshInstance(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const Transform& transform);
        ~MeshInstance();

        void setTransform(Transform& transform) { m_Transform = transform; }

        const std::shared_ptr<Mesh>       getMesh()      const { return m_Mesh; }
        const std::shared_ptr<Material>   getMaterial()  const { return m_Material; }
        const Transform&                  getTransform() const { return m_Transform; }

    private:

        std::shared_ptr<Mesh> m_Mesh;
        std::shared_ptr<Material> m_Material;
        Transform m_Transform;

        int m_ImageIdx = 0;
    };

}

#endif //YARE_MODEL_H
