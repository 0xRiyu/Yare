//
// Created by Drew on 11/6/2019.
//
#ifndef YAREZO_VULKAN_H
#define YAREZO_VULKAN_H


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>

namespace Yarezo {

    class Vulkan {
    public:
        static std::unique_ptr<VkInstance> InitVulkan();
    };
}



#endif //YAREZO_VULKAN_H
