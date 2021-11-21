#ifndef YARE_FPSCAMERA_H
#define YARE_FPSCAMERA_H

#include "Graphics/Camera/Camera.h"

namespace Yare::Graphics {

    class FpsCamera : public Camera {
       public:
        FpsCamera(uint32_t screenWidth, uint32_t screenHeight);
        // Todo
        FpsCamera(const FpsCamera& other);
        virtual ~FpsCamera() override;

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

}  // namespace Yare::Graphics

#endif  // YARE_FPSCAMERA_H
