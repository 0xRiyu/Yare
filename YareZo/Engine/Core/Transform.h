#ifndef YAREZO_TRANSFORM_H
#define YAREZO_TRANSFORM_H

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>

namespace Yarezo {
    class Transform {
    public:
        Transform();
        Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);

        void set(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);
        void setTranslation(float x, float y, float z);
        void setTranslation(const glm::vec3& translation);
        void setRotation(float yaw, float pitch, float roll);
        void setRotation(const glm::vec3& rotation);
        void setScale(float scaleX, float scaleY, float scaleZ);
        void setScale(const glm::vec3& scale);

        glm::vec3 getTranslation() const { return m_Translation; }
        glm::vec3 getRotation()    const { return m_Rotation; }
        glm::vec3 getScale()       const { return m_Scale; }
        glm::mat4 getMatrix()     const;

    private:
        glm::vec3 m_Translation = glm::vec3(0.0f);
        //TODO : Make this a quaternion
        glm::vec3 m_Rotation = glm::vec3(0.0f);
        glm::vec3 m_Scale = glm::vec3(1.0f);

    };
}

#endif //YAREZO_TRANSFORM_H
