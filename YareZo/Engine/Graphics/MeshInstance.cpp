#include "Graphics/MeshInstance.h"
#include "Utilities/IOHelper.h"

namespace Yarezo::Graphics {

    MeshInstance::MeshInstance(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
        :m_Mesh(mesh), m_Material(material) {
    }

    MeshInstance::MeshInstance(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const Transform& transform)
        :m_Mesh(mesh), m_Material(material), m_Transform(transform) {
    }

    MeshInstance::~MeshInstance() {
    }
}
