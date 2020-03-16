//
// Created by Drew on 2019-10-29.
//

#ifndef YAREZO_GLFWWINDOW_H
#define YAREZO_GLFWWINDOW_H

#include <GLFW/glfw3.h>
#include "src/Window.h"

namespace Yarezo {
    namespace Windows {
        class GlfwWindow : public Window {
        public:
            GlfwWindow(WindowProperties& properties);
            virtual ~GlfwWindow();

            virtual void onUpdate() override;
            virtual void closeWindow() override;
            virtual void setKeyInputCallback() override;
            virtual void setMouseInputCallback() override;
            virtual void setScrollInputCallback() override;
            virtual void setFrameBufferResizeCallback() override;
            virtual void setIcon(const std::string& filePath) override;

            inline virtual void* getNativeWindow() const override { return m_Window; };

        private:
            void init();
            GLFWwindow* m_Window;
        };
    }
}

#endif //YAREZO_GLFWWINDOW_H
