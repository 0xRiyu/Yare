#ifndef YARE_CAMERA_H
#define YARE_CAMERA_H

#include "Graphics/Camera/YzCamera.h"

namespace Yare::Graphics {

    class Camera : public YzCamera {

    public:
        Camera(uint32_t screenWidth, uint32_t screenHeight);
        // Todo
        Camera(const Camera& other);
        virtual ~Camera() override;

        // Some functions that just take in a vec 3, Todo, make non vec3 inputs
        virtual void setPosition(const glm::vec3& in) override;
        virtual void setRotation(const glm::vec3& in) override;
        virtual void setLookAt(const glm::vec3& in) override;
        virtual void setFov(float in) override;
        virtual void setCameraSpeed(float speed) override;

        virtual void updateDimensions(uint32_t screenWidth, uint32_t screenHeight) override;

    private:

        void recalculateViewParams();
        void updateView();
        void updateProj();
    };

}

#endif // YARE_CAMERA_H
