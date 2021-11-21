#ifndef YARE_CAMERA_H
#define YARE_CAMERA_H

#include "Graphics/Components/Transform.h"

namespace Yare::Graphics {

    class Camera {
       public:
        virtual ~Camera() = default;
        virtual void updateDimensions(uint32_t screenWidth, uint32_t screenHeight) = 0;

        virtual void setPosition(const glm::vec3& in) = 0;
        virtual void setRotation(const glm::vec3& in) = 0;
        virtual void setLookAt(const glm::vec3& in) = 0;
        virtual void setFov(const float in) = 0;
        virtual void setCameraSpeed(const float speed) = 0;

        // clang-format off
        virtual Transform getTransform()         const { return m_Transform; }
        virtual glm::vec3 getLookAtVector()      const { return m_LookAt; }
        virtual glm::vec3 getUpVector()          const { return m_Up; }
        virtual glm::mat4 getProjectionMatrix()  const { return m_ProjectionMatrix; }
        virtual glm::mat4 getViewMatrix()        const { return m_ViewMatrix; }
        virtual float getFov()                   const { return m_Fov; }
        virtual float getCameraSpeed()           const { return m_CameraSpeed; }
        // clang-format on

       protected:
        Transform m_Transform;

        glm::vec3 m_LookAt = glm::vec3(0.0f);
        glm::vec3 m_Up = glm::vec3(0.0f);
        glm::vec3 m_Front = glm::vec3(0.0f);

        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
        glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

        float m_Aspect = 0.0f;
        float m_Fov = 0.0f;
        float m_CameraSpeed = 0.0f;
    };
}  // namespace Yare::Graphics

#endif  // YARE_CAMERA_H
