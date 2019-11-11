//
// Created by Drew on 11/6/2019.
//
#ifndef YAREZO_VULKAN_H
#define YAREZO_VULKAN_H


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <iostream>

namespace Yarezo {

    class Vulkan {
    public:
        static std::unique_ptr<VkInstance> InitVulkan();
        static bool checkValidationLayerSupport();

    private:
        static VkInstance createInstance();
        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        static std::vector<const char*> getRequiredExtensions();
    };

        const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

}



#endif //YAREZO_VULKAN_H
