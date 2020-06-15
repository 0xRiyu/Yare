#ifndef YARE_WINDOW_H
#define YARE_WINDOW_H

#include "Core/Core.h"
#include "Input/InputHandler.h"
#include "Graphics/Camera/Camera.h"

#include <memory>
#include <string>

namespace Yare::Graphics {

    struct WindowProperties {
        uint32_t width;
        uint32_t height;
    };

    class Window {
    public:
        virtual ~Window() = default;
        virtual void onUpdate() = 0;
        virtual bool shouldClose() = 0;
        virtual void close() = 0;
        virtual void releaseInputHandling() = 0;
        virtual void setWindowProperties(WindowProperties& props) { m_Properties = props; }
        virtual WindowProperties                getWindowProperties()  const { return m_Properties; }
        virtual std::shared_ptr<InputHandler>   getKeyHandler()        const { return m_KeyHandler; }
        virtual std::shared_ptr<InputHandler>   getMouseHandler()      const { return m_MouseHandler; }
        virtual std::shared_ptr<Camera>         getCamera()            const { return m_Camera; }

        static std::shared_ptr<Window> createNewWindow(WindowProperties& properties);
        virtual void* getNativeWindow() const = 0;

        bool windowResized = false;
        bool windowIsFocused = true;
    protected:
        WindowProperties m_Properties;
        std::shared_ptr<InputHandler> m_KeyHandler;
        std::shared_ptr<InputHandler> m_MouseHandler;
        std::shared_ptr<Camera> m_Camera;
    };
}


#endif //YARE_WINDOW_H
