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
        int presentFamily = -1;

        bool isComplete() {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    };


    class GraphicsDevice_Vulkan {
    public:
        GraphicsDevice_Vulkan();

        ~GraphicsDevice_Vulkan();

        void InitVulkan(GLFWwindow* nativeWindow);
        bool checkValidationLayerSupport();

    private:
        VkInstance createInstance();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        std::vector<const char*> getRequiredExtensions();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSurface(GLFWwindow* nativeWindow);

        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);


        const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };



        VkInstance m_Instance;
        VkSurfaceKHR m_Surface;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device;
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;

    #ifdef NDEBUG
            const bool enableValidationLayers = false;
    #else
            const bool enableValidationLayers = true;
    #endif
    };
}



#endif //YAREZO_VULKAN_H
