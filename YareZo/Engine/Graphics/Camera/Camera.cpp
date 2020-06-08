
#include "Graphics/Camera/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Yarezo::Graphics {

    Camera::Camera(uint32_t screenWidth, uint32_t screenHeight) {
        m_Aspect = static_cast<float>(screenWidth) / screenHeight;

        m_Transform = Transform(glm::vec3(1.0f, 2.0f, -4.0f),
                                glm::radians(glm::vec3(90.0f, -20.5f, 0.0f)),
                                glm::vec3(1.0f));

        m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        m_Fov = 50.0f;

        recalculateViewParams();

        updateView();
        updateProj();
    }

    Camera::~Camera() {
    }

    void Camera::setPosition(const glm::vec3& in) {
        m_Transform.setTranslation(in);
        updateView();
    }

    void Camera::setRotation(const glm::vec3& in) {
        m_Transform.setRotation(in);

        recalculateViewParams();
        updateView();
    }

    void Camera::setLookAt(const glm::vec3& in) {
        m_LookAt = in;
        updateView();
    }

    void Camera::setFov(float in) {
        m_Fov = in;
        updateProj();
    }

    void Camera::setCameraSpeed(float speed) {
        m_CameraSpeed = speed;
    }

    void Camera::updateDimensions(uint32_t screenWidth, uint32_t screenHeight) {
        m_Aspect = static_cast<float>(screenWidth) / screenHeight;
        updateProj();
    }

    void Camera::recalculateViewParams() {

        m_LookAt.x = cos(m_Transform.getVec3Rotation().x) * cos(m_Transform.getVec3Rotation().y);
        m_LookAt.y = sin(m_Transform.getVec3Rotation().y);
        m_LookAt.z = sin(m_Transform.getVec3Rotation().x) * cos(m_Transform.getVec3Rotation().y);
        m_LookAt = glm::normalize(m_LookAt);

        auto quat = m_Transform.getQuatRotation();


        m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(m_LookAt, m_Up));
        m_Up = glm::normalize(glm::cross(right, m_LookAt));
    }

    void Camera::updateView() {
        m_ViewMatrix = glm::lookAtRH(m_Transform.getTranslation(), m_Transform.getTranslation() + m_LookAt, m_Up);
    }

    void Camera::updateProj() {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_Aspect, 0.1f, 100.0f);
    }
}
