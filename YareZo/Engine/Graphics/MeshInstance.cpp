#include "Graphics/MeshInstance.h"
#include "Utilities/IOHelper.h"

namespace Yarezo::Graphics {

    MeshInstance::MeshInstance(Mesh* mesh, const std::string& textureFilePath)
        :m_Mesh(mesh), m_TextureFilePath( std::vector{textureFilePath}) {
    }

    MeshInstance::MeshInstance(Mesh* mesh, const std::vector<std::string>& textureFilePaths)
        :m_Mesh(mesh), m_TextureFilePath(textureFilePaths) {
    }

    MeshInstance::~MeshInstance() {
        delete m_Material;
    }

    void MeshInstance::load(MaterialTexType textureType){
        loadMaterial(textureType);
    }

    void MeshInstance::loadMaterial(MaterialTexType textureType) {
        m_Material = new Material(m_TextureFilePath, textureType);

    }

}
