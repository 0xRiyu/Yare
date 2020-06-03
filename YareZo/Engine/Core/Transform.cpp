#include "Core/Transform.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Yarezo {
    Transform::Transform() { }
    Transform::Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale):
        m_Translation(translation), m_Rotation(glm::quat(glm::radians(rotation))), m_Scale(scale) { }

    void Transform::set(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) {
        m_Translation = translation;
        m_Rotation = glm::quat(glm::radians(rotation));
        m_Scale = scale;
    }

    void Transform::setTranslation(float x, float y, float z) {
        m_Translation = glm::vec3(x, y, z);
    }

    void Transform::setTranslation(const glm::vec3& translation) {
        m_Translation = translation;
    }

    void Transform::setRotation(float pitch, float yaw, float roll) {
        m_Rotation = glm::quat(glm::radians(glm::vec3(pitch, yaw, roll)));
    }

    void Transform::setRotation(const glm::vec3& rotation) {
        m_Rotation = glm::quat(glm::radians(rotation));
    }

    void Transform::setRotation(const glm::quat& rotation) {
        m_Rotation = rotation;
    }

    void Transform::setScale(float scaleX, float scaleY, float scaleZ) {
        m_Scale = glm::vec3(scaleX, scaleY, scaleZ);
    }

    void Transform::setScale(const glm::vec3& scale) {
        m_Scale = scale;
    }

    glm::mat4 Transform::getMatrix() const {
        glm::mat4 trans = glm::translate(m_Translation);
        glm::mat4 rot = glm::mat4_cast(m_Rotation);
        glm::mat4 scale = glm::scale(m_Scale);
        return trans * rot * scale;
    }
}
