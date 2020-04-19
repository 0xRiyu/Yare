//
// Created by Drew on 2019-10-29.
//

#include "Platform/Windows/GlfwWindow.h"
#include "Handlers/KeyHandler.h"
#include "Handlers/MouseHandler.h"
#include "Graphics/Camera/Camera.h"
#include "Core/yzh.h"

#include <stb/stb_image.h>

std::shared_ptr<Yarezo::YzWindow> Yarezo::YzWindow::createNewWindow(Yarezo::WindowProperties& properties) {
    return std::make_shared<Yarezo::Windows::GlfwWindow>(properties);
}

namespace Yarezo {
    namespace Windows {

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
            auto app = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
            app->windowResized = true;
        }

        static void GLFWkeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto keyHandler = std::dynamic_pointer_cast<KeyHandler>(Application::getAppInstance()->getWindow()->getKeyHandler());
            keyHandler->m_KeyState[key] = action;
        }

        static void GLFWmouseCallback(GLFWwindow* window, double mouseX, double mouseY) {
            auto mouseHandler = std::dynamic_pointer_cast<MouseHandler>(Application::getAppInstance()->getWindow()->getMouseHandler());
            mouseHandler->currentMouseX = (float)mouseX;
            mouseHandler->currentMouseY = (float)mouseY;
            mouseHandler->mouseEvent = true;
        }

        static void GLFWscrollCallback(GLFWwindow* window, double horizontalScroll, double verticalScroll) {
            auto mouseHandler = std::dynamic_pointer_cast<MouseHandler>(Application::getAppInstance()->getWindow()->getMouseHandler());
            mouseHandler->setHorizontalScroll((float)horizontalScroll);
            mouseHandler->setVerticalScroll((float)verticalScroll);
            mouseHandler->scrollEvent = true;
        }

        GlfwWindow::GlfwWindow(WindowProperties& properties) {
            m_Properties = properties;
            m_Camera = std::make_shared<Camera>(static_cast<float>(m_Properties.width), static_cast<float>(m_Properties.height));
            m_KeyHandler = std::make_shared<KeyHandler>(m_Camera);
            m_MouseHandler = std::make_shared<MouseHandler>(m_Camera);
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
            setIcon("..\\..\\..\\..\\YareZo\\Resources\\Textures\\engineLogo.png");
            setFrameBufferResizeCallback();
            setKeyInputCallback();
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // capture cursor
            setMouseInputCallback();
            setScrollInputCallback();
        }


        void GlfwWindow::onUpdate() {
            glfwPollEvents();
            m_KeyHandler->handle();
            m_MouseHandler->handle();
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
        void GlfwWindow::setIcon(const std::string& filePath) {
            GLFWimage image;
            int imgWidth, imgHeight, imgChannels;
            stbi_uc* pixels = stbi_load(filePath.data(), &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);
            image.height = imgHeight;
            image.width = imgWidth;
            image.pixels = pixels;
            glfwSetWindowIcon(m_Window, 1, &image);
        }
    }
}
