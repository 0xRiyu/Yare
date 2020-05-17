#ifndef YAREZO_MODEL_H
#define YAREZO_MODEL_H

#include "Graphics/Mesh.h"
#include "Graphics/Material.h"

#include <string>

namespace Yarezo::Graphics {

    class Model {
    public:
        Model(const std::string& meshFilePath, const std::string& textureFilePath = "");
        ~Model();

        const Mesh* getMesh() const { return m_Mesh; }
        const Material* getMaterial() const { return m_Material; }

    private:
        void loadMesh();
        void loadMaterial();

        Mesh* m_Mesh;
        Material* m_Material;

        std::string m_MeshFilePath;
        std::string m_TextureFilePath;
    };

}

#endif //YAREZO_MODEL_H
