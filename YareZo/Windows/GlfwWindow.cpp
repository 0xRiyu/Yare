//
// Created by Drew on 2019-10-29.
//

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

#include "GlfwWindow.h"

namespace Yarezo {

    std::shared_ptr<Window> Window::CreateWindow(unsigned short width, unsigned short height) {
        return std::make_shared<GlfwWindow>(width, height);
    }
    
    GlfwWindow::GlfwWindow(unsigned short width, unsigned short height) {
        Init(width, height);
    }

    GlfwWindow::~GlfwWindow() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void GlfwWindow::Init(unsigned short width, unsigned short height) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow(width, height, "Vulkan window", nullptr, nullptr);

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::cout << extensionCount << " extensions supported" << std::endl;

        glm::mat4 matrix;
        glm::vec4 vec;
        auto test = matrix * vec;

    }
    
    void GlfwWindow::OnUpdate() {
        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();
        }
    }


}
