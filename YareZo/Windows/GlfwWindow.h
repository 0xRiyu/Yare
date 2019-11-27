//
// Created by Drew on 2019-10-29.
//

#ifndef YAREZO_GLFWWINDOW_H
#define YAREZO_GLFWWINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Window.h"

namespace Yarezo{
    class GlfwWindow : public Window {
    public:
        GlfwWindow(WindowProperties& properties);
        virtual ~GlfwWindow();
        
        void OnUpdate() override;

        inline virtual WindowProperties getWindowProperties() { return m_Properties; }
        inline virtual void* getNativeWindow() const { return m_Window; };

    private:
        void Init(WindowProperties& properties);
        GLFWwindow* m_Window;
    };
}



#endif //YAREZO_GLFWWINDOW_H
