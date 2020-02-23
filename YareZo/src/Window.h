//
// Created by Drew on 2019-08-17.
//

#ifndef YAREZO_WINDOW
#define YAREZO_WINDOW

#include <memory>

#include "src/Core.h"
#include "src/YzInput.h"
#include "src/YzCamera.h"

namespace Yarezo {
    struct WindowProperties {
        unsigned short width;
        unsigned short height;
    };

    class Window {
    public:
        virtual ~Window() = default;
        virtual void onUpdate() = 0;
        inline virtual WindowProperties getWindowProperties() { return m_Properties; }
        inline virtual InputHandler* getInputHandler() { return &m_InputHandler; }
        inline virtual std::shared_ptr<Camera> getCamera() { return m_Camera; }

        static std::shared_ptr<Window> createNewWindow(WindowProperties& properties);
        virtual void* getNativeWindow() const = 0;

        bool windowResized = false;
    protected:
        WindowProperties m_Properties;
        InputHandler m_InputHandler;
        std::shared_ptr<Camera> m_Camera;
    };
}



#endif //YAREZO_WINDOW
