#include "Graphics/Model.h"
#include "Utilities/IOHelper.h"

namespace Yarezo::Graphics {

    Model::Model(const std::string& filePath, const std::string& textureFilePath)
        :m_MeshFilePath(filePath), m_TextureFilePath(textureFilePath) {
        loadMesh();
        loadMaterial();
    }

    Model::~Model() {
        delete m_Mesh;
        delete m_Material;
    }

    void Model::loadMesh() {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        Utilities::loadModel(m_MeshFilePath, vertices, indices);

        m_Mesh = new Mesh(vertices, indices);
    }

    void Model::loadMaterial() {
        m_Material = new Material();
        if (m_TextureFilePath != "") {
            m_Material->loadTexture(m_TextureFilePath);
        } else {
            m_Material->loadTexture("../YareZo/Resources/Textures/default.jpg");
        }

    }

}
