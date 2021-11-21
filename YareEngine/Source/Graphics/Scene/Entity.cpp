#include "Entity.h"

#include "Utilities/IOHelper.h"

using namespace std;

namespace Yare::Graphics {

    Entity::Entity(shared_ptr<Mesh> mesh, shared_ptr<Material> material) : m_Mesh(mesh), m_Material(material) {}

    Entity::Entity(shared_ptr<Mesh> mesh, shared_ptr<Material> material, const Transform& transform)
        : m_Mesh(mesh), m_Material(material), m_Transform(transform) {}

    Entity::~Entity() {}
}  // namespace Yare::Graphics
