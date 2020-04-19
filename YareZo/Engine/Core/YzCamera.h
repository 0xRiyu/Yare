
#ifndef YAREZO_YZ_CAMERA_H
#define YAREZO_YZ_CAMERA_H

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>

namespace Yarezo {

    class YzCamera {
    public:
        virtual ~YzCamera() = default;
        virtual void updateDimensions(const float screenWidth, const float screenHeight) = 0;

        virtual void setPosition(const glm::vec3& in) = 0;
        virtual void setRotation(const glm::vec3& in) = 0;
        virtual void setLookAt(const glm::vec3& in) = 0;
        virtual void setFov(const float in) = 0;
        virtual void setCameraSpeed(const float speed) = 0;

        virtual glm::vec3 getPosition()          const { return m_Position; }
        virtual glm::vec3 getRotation()          const { return m_Rotation; }
        virtual glm::vec3 getLookAtVector()      const { return m_LookAt; }
        virtual glm::vec3 getUpVector()          const { return m_Up; }
        virtual glm::mat4 getProjectionMatrix()  const { return m_ProjectionMatrix; }
        virtual glm::mat4 getViewMatrix()        const { return m_ViewMatrix; }
        virtual float getFov()                   const { return m_Fov; }
        virtual float getCameraSpeed()           const { return m_CameraSpeed; }

    protected:
        glm::vec3 m_Position;
        glm::vec3 m_Rotation;
        glm::vec3 m_LookAt;
        glm::vec3 m_Up;
        glm::vec3 m_Front;

        float m_Aspect;

        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;

        float m_Fov;
        float m_CameraSpeed = 0.0f;
    };
}


#endif // YAREZO_YZ_CAMERA_H
