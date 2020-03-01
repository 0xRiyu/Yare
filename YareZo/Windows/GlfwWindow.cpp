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

        static void GLFWkeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto keyHandler = Application::getAppInstance()->getWindow()->getKeyHandler();
            keyHandler->m_Keys[key] = action != GLFW_RELEASE;
        }

        static void GLFWmouseCallback(GLFWwindow* window, double mouseX, double mouseY) {
            auto mouseHandler = Application::getAppInstance()->getWindow()->getMouseHandler();
            mouseHandler->mouseX= (float)mouseX;
            mouseHandler->mouseY= (float)mouseY;
        }

        static void GLFWscrollCallback(GLFWwindow* window, double horizontalScroll, double verticalScroll) {
            auto scrollHandler = Application::getAppInstance()->getWindow()->getScrollHandler();
            scrollHandler->horizontalScroll = (float)horizontalScroll;
            scrollHandler->verticalScroll = (float)verticalScroll;

        }


        GlfwWindow::GlfwWindow(WindowProperties& properties) {
            m_Properties = properties;
            m_Camera = std::make_shared<Camera>(static_cast<float>(m_Properties.width), static_cast<float>(m_Properties.height));
            m_keyHandler = std::make_shared<KeyHandler>();
            m_mouseHandler = std::make_shared<MouseHandler>();
            m_scrollHandler = std::make_shared<ScrollHandler>();
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
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // capture cursor
            setMouseInputCallback();
            setScrollInputCallback();
        }


        void GlfwWindow::onUpdate() {
            glfwPollEvents();
            m_keyHandler->handle(m_Camera);
            m_mouseHandler->handle(m_Camera);
            m_scrollHandler->handle(m_Camera);
            windowResized = false;
        }

        void GlfwWindow::closeWindow() {
            glfwSetWindowShouldClose(m_Window, 1);
        }

        void GlfwWindow::setKeyInputCallback() {
            glfwSetKeyCallback(m_Window, GLFWkeyCallback);
        }

        void GlfwWindow::setMouseInputCallback() {
            glfwSetCursorPosCallback(m_Window, GLFWmouseCallback);
        }

        void GlfwWindow::setScrollInputCallback() {
            glfwSetScrollCallback(m_Window, GLFWscrollCallback);
        }

        void GlfwWindow::setFrameBufferResizeCallback() {
            glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallback);
        }
    }
}
