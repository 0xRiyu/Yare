#ifndef YARE_TRANSFORM_H
#define YARE_TRANSFORM_H

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Component.h"

namespace Yare {
    class Transform : public Component {
       public:
        Transform();
        Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);

        void set(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);
        void setTranslation(float x, float y, float z);
        void setTranslation(const glm::vec3& translation);
        void setRotation(float pitch, float yaw, float roll);
        void setRotation(const glm::vec3& rotation);
        void setRotation(const glm::quat& rotation);
        void setScale(float scaleX, float scaleY, float scaleZ);
        void setScale(const glm::vec3& scale);

        glm::vec3 getTranslation() const { return m_Translation; }
        glm::vec3 getVec3Rotation() const { return glm::eulerAngles(m_Rotation); }
        glm::quat getQuatRotation() const { return m_Rotation; }
        glm::vec3 getScale() const { return m_Scale; }
        glm::mat4 getMatrix() const { return m_Matrix; };

       private:
        void updateMatrix();

        glm::vec3 m_Translation = glm::vec3(0.0f);
        glm::quat m_Rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 m_Scale = glm::vec3(1.0f);
        glm::mat4 m_Matrix = glm::mat4(1.0);
    };
}  // namespace Yare

#endif  // YARE_TRANSFORM_H
