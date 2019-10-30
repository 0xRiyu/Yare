//
// Created by Drew on 2019-08-17.
//

#ifndef YAREZO_WINDOW
#define YAREZO_WINDOW

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include "Core.h"

namespace Yarezo{
    class YAREZO_API Window {
      
    public:

        virtual ~Window() = default;
        virtual void OnUpdate() = 0;
        
        static std::shared_ptr<Window> CreateWindow(unsigned short width, unsigned short height);
        
    };
}



#endif //YAREZO_WINDOW
