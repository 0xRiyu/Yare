//
// Created by Drew on 2019-10-29.
//

#include "Windows/GlfwWindow.h"
#include "src/Application.h"
#include "Utilities/YzLogger.h"

std::shared_ptr<Yarezo::Window> Yarezo::Window::createNewWindow(Yarezo::WindowProperties& properties) {
    return std::make_shared<Yarezo::Windows::GlfwWindow>(properties);
}

namespace Yarezo {
    namespace Windows {

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
            auto app = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
            app->windowResized = true;
        }

        static void GLFWcallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto inputHandler = Application::getAppInstance()->getWindow()->getInputHandler();
            inputHandler->m_Keys[key] = action != GLFW_RELEASE;
        }


        GlfwWindow::GlfwWindow(WindowProperties& properties) {
            m_Properties = properties;
            m_Camera = std::make_shared<Camera>(static_cast<float>(m_Properties.width), static_cast<float>(m_Properties.height));
            m_InputHandler = std::make_shared<InputHandler>();
            init();
        }

        GlfwWindow::~GlfwWindow() {
            glfwDestroyWindow(m_Window);
            glfwTerminate();
        }

        void GlfwWindow::init() {
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            m_Window = glfwCreateWindow(m_Properties.width, m_Properties.height, "YareZo!", nullptr, nullptr);
            glfwSetWindowUserPointer(m_Window, this);
            setFrameBufferResizeCallback();
            setKeyInputCallback();
        }


        void GlfwWindow::onUpdate() {
            glfwPollEvents();
            m_InputHandler->Handle(m_Camera);
            windowResized = false;
        }

        void GlfwWindow::closeWindow() {
            glfwSetWindowShouldClose(m_Window, 1);
        }

        void GlfwWindow::setKeyInputCallback() {
            glfwSetKeyCallback(m_Window, GLFWcallback);
        }

        void GlfwWindow::setFrameBufferResizeCallback() {
            glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallback);
        }

    }
}
