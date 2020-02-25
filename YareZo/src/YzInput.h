#ifndef YZINPUT_HPP
#define YZINPUT_HPP

#include "src/YzCamera.h"
#include <GLFW/glfw3.h>

#include <memory>

#define MAX_BUTTONS GLFW_MOUSE_BUTTON_LAST
#define MAX_KEYS GLFW_KEY_LAST

namespace Yarezo {

    class InputHandler {
    public:

        InputHandler();
        virtual ~InputHandler();

        virtual bool isKeyDown(int key);
        virtual void Handle(std::shared_ptr<Camera> currentCamera);

        bool m_Keys[MAX_KEYS];
        bool m_Buttons[MAX_BUTTONS];

    private:

    };


}
#endif