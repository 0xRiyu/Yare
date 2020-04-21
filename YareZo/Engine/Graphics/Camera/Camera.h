
#ifndef YAREZO_CAMERA_H
#define YAREZO_CAMERA_H

#include "Core/YzCamera.h"

namespace Yarezo::Graphics {

    class Camera : public YzCamera {

    public:
        Camera(const float screenWidth, const float screenHeight);
        // Todo
        Camera(const Camera& other);
        virtual ~Camera() override;

        // Some functions that just take in a vec 3, Todo, make non vec3 inputs
        virtual void setPosition(const glm::vec3& in) override;
        virtual void setRotation(const glm::vec3& in) override;
        virtual void setLookAt(const glm::vec3& in) override;
        virtual void setFov(const float in) override;
        virtual void setCameraSpeed(const float speed) override;

        virtual void updateDimensions(const float screenWidth, const float screenHeight) override;

    private:

        void recalculateViewParams();
        void updateView();
        void updateProj();
    };

}

#endif // YAREZO_CAMERA_H
