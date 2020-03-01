//
// Created by Drew on 2019-08-17.
//

#ifndef YAREZO_WINDOW
#define YAREZO_WINDOW

#include "src/Core.h"
#include "src/handlers/YzKeyHandler.h"
#include "src/handlers/YzMouseHandler.h"
#include "src/YzCamera.h"

#include <memory>

namespace Yarezo {

    struct WindowProperties {
        unsigned short width;
        unsigned short height;
    };

    class Window {
    public:
        virtual ~Window() = default;
        virtual void onUpdate() = 0;
        virtual void closeWindow() = 0;
        virtual void setKeyInputCallback() = 0;
        virtual void setMouseInputCallback() = 0;
        virtual void setScrollInputCallback() = 0;
        virtual void setFrameBufferResizeCallback() = 0;
        inline virtual WindowProperties getWindowProperties()                   { return m_Properties; }
        inline virtual std::shared_ptr<KeyHandler> getKeyHandler()          { return m_keyHandler; }
        inline virtual std::shared_ptr<MouseHandler> getMouseHandler()          { return m_mouseHandler; }
        inline virtual std::shared_ptr<Camera> getCamera()                      { return m_Camera; }

        static std::shared_ptr<Window> createNewWindow(WindowProperties& properties);
        virtual void* getNativeWindow() const = 0;

        bool windowResized = false;
    protected:
        WindowProperties m_Properties;
        std::shared_ptr<KeyHandler> m_keyHandler;
        std::shared_ptr<MouseHandler> m_mouseHandler;
        std::shared_ptr<Camera> m_Camera;
    };
}



#endif //YAREZO_WINDOW
