#ifndef YAREZO_KEYHANDLER_H
#define YAREZO_KEYHANDLER_H

#include "Core/YzInputHandler.h"
#include "Core/Glfw.h"
#include "Core/YzCamera.h"

#include <memory>

#define MAX_BUTTONS GLFW_MOUSE_BUTTON_LAST
#define MAX_KEYS GLFW_KEY_LAST

namespace Yarezo {

    class KeyHandler: public YzInputHandler {
    public:
        KeyHandler(std::shared_ptr<YzCamera> currentCamera);
        virtual ~KeyHandler();

        virtual bool isKeyDown(int key);
        virtual bool isKeyPressed(int key);
        virtual void handle() override;

        int  m_KeyState[MAX_KEYS];
        bool m_Buttons[MAX_BUTTONS];

    private:
        std::shared_ptr<YzCamera> p_Camera;

    };

}
#endif //YAREZO_KEYHANDLER_H
