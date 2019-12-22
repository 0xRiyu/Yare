#ifndef YZINPUT_HPP
#define YZINPUT_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <src/YzCamera.h>

#include <memory>

#define MAX_BUTTONS GLFW_MOUSE_BUTTON_LAST
#define MAX_KEYS GLFW_KEY_LAST

namespace Yarezo {

    class InputHandler {
    public:

        InputHandler();
        virtual ~InputHandler();

        virtual bool isKeyDown(int key);

        virtual void setupKeyInputs(GLFWwindow* window);

        virtual void Handle(std::shared_ptr<Camera> currentCamera);


        bool m_Keys[MAX_KEYS];
        bool m_Buttons[MAX_BUTTONS];

    private:

        static void GLFWcallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    };


}
#endif