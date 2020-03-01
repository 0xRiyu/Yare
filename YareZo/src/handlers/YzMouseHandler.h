#ifndef YZMOUSE_HPP
#define YZMOUSE_HPP

#include "src/YzCamera.h"
#include <GLFW/glfw3.h>
#include "src/handlers/Handler.h"

#include <memory>


namespace Yarezo {

    class MouseHandler: public Handler {
    public:

        MouseHandler();
        virtual ~MouseHandler();

        virtual void handle(std::shared_ptr<Camera> currentCamera) override;

        bool isFirstMouseInput = true;
        float mouseX;
        float mouseY;
        float prevMouseX;
        float prevMouseY;
        const float mouseSensitivity = 0.1f;

    private:

    };


}
#endif