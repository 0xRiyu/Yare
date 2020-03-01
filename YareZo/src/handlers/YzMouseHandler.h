#ifndef YZMOUSE_HPP
#define YZMOUSE_HPP

#include "src/YzCamera.h"
#include <GLFW/glfw3.h>

#include <memory>

#define MAX_BUTTONS GLFW_MOUSE_BUTTON_LAST
#define MAX_KEYS GLFW_KEY_LAST

namespace Yarezo {

    class MouseHandler {
    public:

        MouseHandler();
        virtual ~MouseHandler();

        virtual void Handle(std::shared_ptr<Camera> currentCamera);

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