//
// Created by Drew on 2019-10-29.
//

#include "Windows/GlfwWindow.h"

namespace Yarezo {

    std::shared_ptr<Window> Window::CreateNewWindow(WindowProperties& properties) {
        return std::make_shared<GlfwWindow>(properties);
    }
    
    GlfwWindow::GlfwWindow(WindowProperties& properties) {
        m_Properties = properties;
        Init(properties);
    }

    GlfwWindow::~GlfwWindow() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void GlfwWindow::Init(WindowProperties& properties) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow(properties.width, properties.height, "YareZo!", nullptr, nullptr);
    }
    
    void GlfwWindow::OnUpdate() {
        glfwPollEvents();
    }


}
