#ifndef YAREZO_MOUSEHANDER_H
#define YAREZO_MOUSEHANDER_H

#include "Core/YzInputHandler.h"
#include "Core/YzCamera.h"

#include <memory>

namespace Yarezo {

    class MouseHandler: public YzInputHandler {

    public:
        MouseHandler(const std::shared_ptr<YzCamera> currentCamera);
        virtual ~MouseHandler();

        virtual void handle() override;

        virtual void handleMouseEvent();
        virtual void handleScrollEvent();

        virtual void setHorizontalScroll(const float val) { m_HorizontalScroll = val; }
        virtual void setVerticalScroll(const float val) { m_VerticalScroll = val; }

        bool mouseEvent = false;
        bool scrollEvent = false;

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
#endif //YAREZO_MOUSEHANDER_H
