//
// Created by Drew on 2019-10-29.
//

#ifndef YAREZO_GLFWWINDOW_H
#define YAREZO_GLFWWINDOW_H

#include <GLFW/glfw3.h>
#include "src/Window.h"

namespace Yarezo{
    class GlfwWindow : public Window {
    public:
        GlfwWindow(WindowProperties& properties);
        virtual ~GlfwWindow();
        
        void onUpdate() override;

        inline virtual void* getNativeWindow() const { return m_Window; };

    private:
        void init();
        GLFWwindow* m_Window;
    };
}

#endif //YAREZO_GLFWWINDOW_H
