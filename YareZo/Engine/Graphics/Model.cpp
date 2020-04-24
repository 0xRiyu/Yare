#include "Graphics/Model.h"
#include "Utilities/IOHelper.h"

namespace Yarezo::Graphics {

    Model::Model(const std::string& filePath)
        :m_FilePath(filePath) {
        load();
    }

    Model::~Model() {
        delete m_Mesh;
    }

    void Model::load() {

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        Utilities::loadModel(m_FilePath, vertices, indices);

        m_Mesh = new Mesh(vertices, indices);
    }


}
