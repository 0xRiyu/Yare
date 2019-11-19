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

    struct QueueFamilyIndices {
        int graphicsFamily = -1;

        bool isComplete() {
            return graphicsFamily >= 0;
        }
    };


    class GraphicsDevice_Vulkan {
    public:
        GraphicsDevice_Vulkan();

        ~GraphicsDevice_Vulkan();

        void InitVulkan();
        bool checkValidationLayerSupport();

    private:
        VkInstance createInstance();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        std::vector<const char*> getRequiredExtensions();
        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);


        const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

        VkInstance m_instance;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;

    };
}



#endif //YAREZO_VULKAN_H
