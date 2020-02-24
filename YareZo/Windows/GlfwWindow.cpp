//
// Created by Drew on 2019-10-29.
//

#include "Windows/GlfwWindow.h"
#include "Utilities/YzLogger.h"

namespace Yarezo {

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        app->windowResized = true;
    }

    std::shared_ptr<Window> Window::createNewWindow(WindowProperties& properties) {
        return std::make_shared<GlfwWindow>(properties);
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
        glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallback);
        m_InputHandler->setupKeyInputs(m_Window);
    }
    
    void GlfwWindow::onUpdate() {
        glfwPollEvents();
        m_InputHandler->Handle(m_Camera);
        windowResized = false;
    }

}
