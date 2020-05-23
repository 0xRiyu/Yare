#ifndef YAREZO_MODEL_H
#define YAREZO_MODEL_H

#include "Graphics/Mesh.h"
#include "Graphics/Material.h"

#include <string>
#include <vector>

namespace Yarezo::Graphics {

    class Model {
    public:
        Model(const std::string& meshFilePath, const std::string& textureFilePath = "");
        Model(const std::string& meshFilePath, const std::vector<std::string>& textureFilesPaths);
        ~Model();

        void load(MaterialTexType textureType);

        const Mesh* getMesh() const { return m_Mesh; }
        const Material* getMaterial() const { return m_Material; }
        void setImageIdx(int Idx) { m_ImageIdx = Idx; }
        int getImageIdx() const { return m_ImageIdx; }

    private:
        void loadMesh();
        void loadMaterial(MaterialTexType textureType);

        Mesh* m_Mesh;
        Material* m_Material;

        std::string m_MeshFilePath;
        std::vector<std::string> m_TextureFilePath;

        int m_ImageIdx = 0;
    };

}

#endif //YAREZO_MODEL_H
