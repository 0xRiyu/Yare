//
// Created by Drew on 2019-10-29.
//

#ifndef YAREZO_GLFWWINDOW_H
#define YAREZO_GLFWWINDOW_H

#include "src/Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Yarezo{
    class GlfwWindow : public Window {
    public:
        GlfwWindow(WindowProperties& properties);
        virtual ~GlfwWindow();
        
        void onUpdate() override;

        inline virtual WindowProperties getWindowProperties() { return m_Properties; }
        inline virtual void* getNativeWindow() const { return m_Window; };

        bool windowResized = false;
    private:
        void init(WindowProperties& properties);
        GLFWwindow* m_Window;
    };
}



#endif //YAREZO_GLFWWINDOW_H
