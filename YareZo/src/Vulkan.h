//
// Created by Drew on 11/6/2019.
//
#ifndef YAREZO_VULKAN_H
#define YAREZO_VULKAN_H

#include <vector>
#include <memory>
#include <iostream>
#include <glm.hpp>
#include <array>

#include "Windows/GlfwWindow.h"

namespace Yarezo {

    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription = {};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            return attributeDescriptions;
        }
    };

    struct QueueFamilyIndices {
        int graphicsFamily = -1;
        int presentFamily = -1;

        bool isComplete() {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class GraphicsDevice_Vulkan {
    public:
        GraphicsDevice_Vulkan(Window* nativeWindow);

        ~GraphicsDevice_Vulkan();

        void initVulkan();
        void drawFrame();
        void waitIdle();

    private:

        void cleanupSwapChain();

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();

        void createInstance();
        void setupDebugMessenger();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSurface();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createGraphicsPipeline();
        void createFramebuffers();
        void createCommandPool();
        void createVertexBuffer();
        void createCommandBuffers();
        void createSyncObjects();
        void recreateSwapChain();

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        // Swap Chain required functions
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkShaderModule createShaderModule(const std::vector<char>& shader_code);


        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        const std::vector<const char*> deviceExtensions {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        const int MAX_FRAMES_IN_FLIGHT = 2;

        // Demo Triangle
        const std::vector<Vertex> vertices = {
            {{0.0f, -0.5f}, {0.556f, 0.164f, 0.321f}},
            {{0.5f, 0.5f}, {0.0f, 0.203f, 0.584f}},
            {{-0.5f, 0.5f}, {0.521f, 0.490f, 0.215f}}
        };


        // Class members created by GraphicsDevice_Vulkan class
        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;
        VkSurfaceKHR m_Surface;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device;
        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;
        VkSwapchainKHR m_SwapChain;
        std::vector<VkImage> m_SwapChainImages;
        VkFormat m_SwapChainImageFormat;
        VkExtent2D m_SwapChainExtent;
        std::vector<VkImageView> m_SwapChainImageViews;
        VkRenderPass m_RenderPass;
        VkPipelineLayout m_PipelineLayout;
        VkPipeline m_GraphicsPipeline;
        std::vector<VkFramebuffer> m_SwapChainFramebuffers;
        VkCommandPool m_CommandPool;
        std::vector<VkCommandBuffer> m_CommandBuffers;
        std::vector<VkSemaphore> m_ImageAvailableSemaphore;
        std::vector<VkSemaphore> m_RenderFinishedSemaphore;
        std::vector<VkFence> m_InFlightFences;
        std::vector<VkFence> m_ImagesInFlight;
        size_t m_CurrentFrame = 0;
        VkBuffer m_VertexBuffer;
        VkDeviceMemory m_VertexBufferMemory;

        // Objects from outside this class
        Window* m_NativeWindow;

    #ifdef NDEBUG
            const bool enableValidationLayers = false;
    #else
            const bool enableValidationLayers = true;
    #endif
    };
}



#endif //YAREZO_VULKAN_H
