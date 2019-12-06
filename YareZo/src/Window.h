//
// Created by Drew on 2019-08-17.
//

#ifndef YAREZO_WINDOW
#define YAREZO_WINDOW

#include <memory>

#include "src/Core.h"

namespace Yarezo {
    struct WindowProperties {
        unsigned short width;
        unsigned short height;
    };

    class Window {
      
    public:

        virtual ~Window() = default;
        virtual void onUpdate() = 0;
        virtual WindowProperties getWindowProperties() = 0;

        static std::shared_ptr<Window> createNewWindow(WindowProperties& properties);
        virtual void* getNativeWindow() const = 0;

    protected:
        WindowProperties m_Properties;
    };
}



#endif //YAREZO_WINDOW
