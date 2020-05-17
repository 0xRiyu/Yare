
#include "Graphics/Camera/Camera.h"
#include "Utilities/YzLogger.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Yarezo::Graphics {

    Camera::Camera(const float screenWidth, const float screenHeight) {
        m_Aspect = screenWidth / screenHeight;
        m_Position = glm::vec3(3.0f, 2.0f, 0.0f);
        m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        m_Fov = 50.0f;

        // TODO: World has X coming into the camera rather than Z
        float yaw = 0.0f;
        float pitch = 32.5f;
        float roll = 0.0f;
        m_Rotation = glm::vec3(yaw, pitch, roll);

        recalculateViewParams();

        updateView();
        updateProj();
    }

    Camera::~Camera() {
    }

    void Camera::setPosition(const glm::vec3& in) {
        m_Position = in;
        updateView();
    }

    void Camera::setRotation(const glm::vec3& in) {
        m_Rotation = in;

        recalculateViewParams();
        updateView();
    }

    void Camera::setLookAt(const glm::vec3& in) {
        m_LookAt = in;
        updateView();
    }

    void Camera::setFov(const float in) {
        m_Fov = in;
        updateProj();
    }

    void Camera::setCameraSpeed(const float speed) {
        m_CameraSpeed = speed;
    }

    void Camera::updateDimensions(const float screenWidth, const float screenHeight) {
        m_Aspect = screenWidth / screenHeight;
        updateProj();
    }

    void Camera::recalculateViewParams() {
        m_LookAt.x = cos(glm::radians(m_Rotation.x)) * cos(glm::radians(m_Rotation.y));
        m_LookAt.y = sin(glm::radians(m_Rotation.y));
        m_LookAt.z = sin(glm::radians(m_Rotation.x)) * cos(glm::radians(m_Rotation.y));
        m_LookAt = glm::normalize(m_LookAt);

        m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(m_LookAt, m_Up));
        m_Up = glm::normalize(glm::cross(right, m_LookAt));
    }

    void Camera::updateView() {
        m_ViewMatrix = glm::lookAtLH(m_Position, m_Position + m_LookAt, m_Up);
    }

    void Camera::updateProj() {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_Aspect, 0.1f, 100.0f);
    }
}
