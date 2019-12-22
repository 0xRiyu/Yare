
#include "src/YzCamera.h"

#include <gtc/matrix_transform.hpp>

namespace Yarezo {

    Camera::Camera(const float screenWidth, const float screenHeight) {
        m_Aspect = screenWidth / screenHeight;
        m_Position = glm::vec3(0.0f, 2.0f, 2.0f);
        m_LookAt = glm::vec3(0.0f, 0.5f, 0.5f);
        m_Up = glm::vec3(0.0f, 0.0f, 1.0f);

        m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

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
        updateView();
    }

    void Camera::updateDimensions(const float screenWidth, const float screenHeight) {
        m_Aspect = screenWidth / screenHeight;
        updateProj();
    }

    void Camera::updateView() {
        m_ViewMatrix = glm::lookAtLH(m_Position, m_Position + m_LookAt, m_Up);
    }

    void Camera::updateProj() {
        m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), m_Aspect, 0.1f, 10.0f);
    }
}