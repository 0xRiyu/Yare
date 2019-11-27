//
// Created by Drew on 2019-08-17.
//

#ifndef YAREZO_WINDOW
#define YAREZO_WINDOW

#include <memory>
#include "Core.h"

namespace Yarezo {
    struct WindowProperties {
        unsigned short width;
        unsigned short height;
    };

    class YAREZO_API Window {
      
    public:

        virtual ~Window() = default;
        virtual void OnUpdate() = 0;
        virtual WindowProperties getWindowProperties() = 0;

        static std::shared_ptr<Window> CreateWindow(WindowProperties& properties);
        virtual void* getNativeWindow() const = 0;

    protected:
        WindowProperties m_Properties;
    };
}



#endif //YAREZO_WINDOW
