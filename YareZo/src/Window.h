//
// Created by Drew on 2019-08-17.
//

#ifndef YAREZO_WINDOW
#define YAREZO_WINDOW

#include <memory>

#include "src/Core.h"
#include "src/YzInput.h"

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

        static std::shared_ptr<Window> createNewWindow(WindowProperties& properties);
        virtual void* getNativeWindow() const = 0;

    protected:
        WindowProperties m_Properties;
        InputHandler m_InputHandler;
    };
}



#endif //YAREZO_WINDOW
