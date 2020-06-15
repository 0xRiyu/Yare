#ifndef YARE_KEYHANDLER_H
#define YARE_KEYHANDLER_H

#include "Graphics/Camera/Camera.h"
#include "Input/InputHandler.h"
#include "Core/Glfw.h"

#include <memory>

#define MAX_BUTTONS GLFW_MOUSE_BUTTON_LAST
#define MAX_KEYS GLFW_KEY_LAST

namespace Yare {
    class KeyHandler: public InputHandler {
    public:
        KeyHandler(std::shared_ptr<Graphics::Camera> currentCamera);
        virtual ~KeyHandler();

        virtual bool isKeyDown(int key);
        virtual bool isKeyPressed(int key);
        virtual void handle() override;

        int  m_KeyState[MAX_KEYS];
        bool m_Buttons[MAX_BUTTONS];

    private:
        std::shared_ptr<Graphics::Camera> p_Camera;

    };
}

#endif //YARE_KEYHANDLER_H
