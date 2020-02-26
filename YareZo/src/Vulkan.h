//
// Created by Drew on 11/6/2019.
//
#ifndef YAREZO_VULKAN_H
#define YAREZO_VULKAN_H
#define GLM_FORCE_RADIANS

#include <vector>
#include <memory>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <array>
#include <chrono>

#include "Platform/Vulkan/Vk_Instance.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Platform/Vulkan/Vk_Swapchain.h"
#include "Platform/Vulkan/Vk_RenderPass.h"
#include "Platform/Vulkan/Vk_Pipeline.h"


namespace Yarezo {

 

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };


    class GraphicsDevice_Vulkan {
    public:
        GraphicsDevice_Vulkan();

        ~GraphicsDevice_Vulkan();

        void initVulkan();
        void drawFrame();
        void waitIdle();

    private:
        void cleanupSwapChain();
        void createGraphicsPipeline();
        void createFramebuffers();
        void createCommandPool();
        void createVertexBuffer();
        void createIndexBuffer();
        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
        void createCommandBuffers();
        void createSyncObjects();
        void recreateSwapChain();
        void updateUniformBuffer(uint32_t currentImage);

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        // Swap Chain required functions
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkShaderModule createShaderModule(const std::vector<char>& shader_code);

        const int MAX_FRAMES_IN_FLIGHT = 2;

        // Demo Rectangle
        const std::vector<Graphics::Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
            {{0.5f, -0.5f}, {0.556f, 0.164f, 0.321f}},
            {{0.5f, 0.5f}, {0.0f, 0.203f, 0.584f}},
            {{-0.5f, 0.5f}, {0.521f, 0.490f, 0.215f}}
        };

        const std::vector<uint16_t> indices = {
            0,1,2,2,3,0
        };


        // Class members created by GraphicsDevice_Vulkan class
        Graphics::YzVkInstance m_VkInstance;
        Graphics::YzVkDevice* m_VkDevice;
        Graphics::YzVkSwapchain m_VkSwapchain;
        Graphics::YzVkRenderPass m_VkRenderPass;
        Graphics::YzVkPipeline m_Pipeline;

        std::vector<VkFramebuffer> m_SwapChainFramebuffers;
        VkCommandPool m_CommandPool;
        std::vector<VkCommandBuffer> m_CommandBuffers;
        std::vector<VkSemaphore> m_ImageAvailableSemaphore;
        std::vector<VkSemaphore> m_RenderFinishedSemaphore;
        std::vector<VkFence> m_InFlightFences;
        std::vector<VkFence> m_ImagesInFlight;
        size_t m_CurrentFrame = 0;
        VkDeviceMemory m_VertexBufferMemory;
        VkDeviceMemory m_IndexBufferMemory;
        std::vector<VkDeviceMemory> m_UniformBuffersMemory;
        VkBuffer m_VertexBuffer;
        VkBuffer m_IndexBuffer;
        std::vector<VkBuffer> m_UniformBuffers;
        VkDescriptorPool m_DescriptorPool;
        std::vector<VkDescriptorSet> m_DescriptorSets;

    };
}



#endif //YAREZO_VULKAN_H