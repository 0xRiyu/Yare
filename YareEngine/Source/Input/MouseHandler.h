#ifndef YARE_MOUSEHANDER_H
#define YARE_MOUSEHANDER_H

#include "Input/YzInputHandler.h"
#include "Graphics/Camera/YzCamera.h"

#include <memory>

namespace Yare {

    class MouseHandler: public YzInputHandler {

    public:
        MouseHandler(const std::shared_ptr<YzCamera> currentCamera);
        virtual ~MouseHandler();

        virtual void handle() override;

        virtual void handleMouseEvent();
        virtual void handleScrollEvent();
        virtual void handleButtonEvent();

        virtual void setHorizontalScroll(const float val) { m_HorizontalScroll = val; }
        virtual void setVerticalScroll(const float val) { m_VerticalScroll = val; }

        bool mouseEvent = false;
        bool scrollEvent = false;
        bool buttonEvent = false;

        bool mouseLeftButtonPressed = false;
        bool mouseRightButtonPressed = false;

        float currentMouseX = 0.0f;
        float currentMouseY = 0.0f;
        float prevMouseX = 0.0f;
        float prevMouseY = 0.0f;

    private:
        std::shared_ptr<YzCamera> p_Camera;
        const float mouseSensitivity = 0.1f;
        float m_HorizontalScroll = 0.0f;
        float m_VerticalScroll = 0.0f;
    };
}
#endif //YARE_MOUSEHANDER_H
