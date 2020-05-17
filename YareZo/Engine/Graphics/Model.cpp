#include "Graphics/Model.h"
#include "Utilities/IOHelper.h"

namespace Yarezo::Graphics {

    Model::Model(const std::string& filePath, const std::string& textureFilePath)
        :m_MeshFilePath(filePath), m_TextureFilePath(textureFilePath) {
    }

    Model::~Model() {
        delete m_Mesh;
        delete m_Material;
    }

    void Model::load(){
        loadMesh();
        loadMaterial();
    }

    void Model::loadMesh() {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        Utilities::loadModel(m_MeshFilePath, vertices, indices);

        m_Mesh = new Mesh(vertices, indices);
    }

    void Model::loadMaterial() {
        m_Material = new Material(m_TextureFilePath);

    }

}
