#ifndef YAREZO_MESH_INSTANCE_H
#define YAREZO_MESH_INSTANCE_H

#include "Graphics/Mesh.h"
#include "Graphics/Material.h"

#include "Core/Transform.h"

#include <string>
#include <vector>

namespace Yarezo::Graphics {

    class MeshInstance {
    public:
        MeshInstance(Mesh* mesh, const std::string& textureFilePath = "");
        MeshInstance(Mesh* mesh, const std::vector<std::string>& textureFilesPaths);
        ~MeshInstance();

        void load(MaterialTexType textureType);

        void setImageIdx(int Idx) { m_ImageIdx = Idx; }
        void setTransform(Transform& transform) { m_Transform = transform; }

        const Mesh*       getMesh()      const { return m_Mesh; }
        const Material*   getMaterial()  const { return m_Material; }
        int               getImageIdx()  const { return m_ImageIdx; }
        Transform         getTransform() const { return m_Transform; }


    private:
        void loadMaterial(MaterialTexType textureType);

        Mesh* m_Mesh;
        Material* m_Material;
        Transform m_Transform;

        std::string m_MeshFilePath;
        std::vector<std::string> m_TextureFilePath;

        int m_ImageIdx = 0;
    };

}

#endif //YAREZO_MODEL_H
