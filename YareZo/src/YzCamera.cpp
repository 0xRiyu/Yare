
#include "src/YzCamera.h"

#include <gtc/matrix_transform.hpp>
#include "Utilities/YzLogger.h"


namespace Yarezo {
    long frameCounter = 0l;

    Camera::Camera(const float screenWidth, const float screenHeight) {
        m_Aspect = screenWidth / screenHeight;
        m_Position = glm::vec3(0.0f, 2.0f, 2.0f);
        m_LookAt = glm::vec3(0.0f, 0.5f, 0.5f);
        // m_Up = glm::vec3(0.0f, 0.0f, 1.0f); // not gonna ask why up was +Z
        m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

        m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

        float yaw = -90.0f;
        float pitch = 0.0f;
        m_fov = 45.0f;

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

    void Camera::setLookAt(const glm::vec3& in) {
        m_LookAt = in;
        updateView();
    }

    void Camera::setFov(const float in) {
        m_fov = in;
        updateProj();
    }
    void Camera::setYaw(const float in) {
        m_yaw = in;
        updateProj();
    }
    void Camera::setPitch(const float in) {
        m_pitch = in;
        updateProj();
    }

    void Camera::updateDimensions(const float screenWidth, const float screenHeight) {
        m_Aspect = screenWidth / screenHeight;
        updateProj();
    }

    void Camera::updateView() {
        frameCounter++;
        if (frameCounter % 1000 == 0) YZ_INFO("LookAtX: " + STR(m_LookAt.x) + "\tLookAtY: " + STR(m_LookAt.y) + "\tLookAtZ: " + STR(m_LookAt.z));
        m_ViewMatrix = glm::lookAtLH(m_Position, m_Position + m_LookAt, m_Up);
    }

    void Camera::updateProj() {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_fov), m_Aspect, 0.1f, 100.0f);
    }
}