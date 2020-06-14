#include "Graphics/MeshInstance.h"
#include "Utilities/IOHelper.h"

using namespace std;

namespace Yare::Graphics {

    MeshInstance::MeshInstance(shared_ptr<Mesh> mesh, shared_ptr<Material> material)
        :m_Mesh(mesh), m_Material(material) {
    }

    MeshInstance::MeshInstance(shared_ptr<Mesh> mesh, shared_ptr<Material> material, const Transform& transform)
        :m_Mesh(mesh), m_Material(material), m_Transform(transform) {
    }

    MeshInstance::~MeshInstance() {
    }
}
