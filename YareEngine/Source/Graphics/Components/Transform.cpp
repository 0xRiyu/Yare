#include "Transform.h"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

namespace Yare {
    Transform::Transform() {}
    Transform::Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
        : m_Translation(translation), m_Rotation(glm::quat(rotation)), m_Scale(scale) {
        updateMatrix();
    }

    void Transform::set(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) {
        m_Translation = translation;
        m_Rotation = glm::quat(rotation);
        m_Scale = scale;
        updateMatrix();
    }

    void Transform::setTranslation(float x, float y, float z) {
        m_Translation = glm::vec3(x, y, z);
        updateMatrix();
    }

    void Transform::setTranslation(const glm::vec3& translation) {
        m_Translation = translation;
        updateMatrix();
    }

    void Transform::setRotation(float pitch, float yaw, float roll) {
        m_Rotation = glm::quat(glm::radians(glm::vec3(pitch, yaw, roll)));
        updateMatrix();
    }

    void Transform::setRotation(const glm::vec3& rotation) {
        m_Rotation = glm::quat(rotation);
        updateMatrix();
    }

    void Transform::setRotation(const glm::quat& rotation) {
        m_Rotation = rotation;
        updateMatrix();
    }

    void Transform::setScale(float scaleX, float scaleY, float scaleZ) {
        m_Scale = glm::vec3(scaleX, scaleY, scaleZ);
        updateMatrix();
    }

    void Transform::setScale(const glm::vec3& scale) {
        m_Scale = scale;
        updateMatrix();
    }

    void Transform::updateMatrix() {
        m_Matrix = glm::translate(m_Translation) * glm::mat4_cast(m_Rotation) * glm::scale(m_Scale);
    }
}  // namespace Yare
