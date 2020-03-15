//
// Created by Drew on 11/6/2019.
//
#ifndef YAREZO_VULKAN_H
#define YAREZO_VULKAN_H
#define GLM_FORCE_RADIANS

#include <vector>
#include <memory>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <array>
#include <chrono>

#include "Platform/Vulkan/Vk_Instance.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Platform/Vulkan/Vk_Swapchain.h"
#include "Platform/Vulkan/Vk_RenderPass.h"
#include "Platform/Vulkan/Vk_Pipeline.h"
#include "Platform/Vulkan/Vk_Framebuffer.h"
#include "Platform/Vulkan/Vk_CommandPool.h"
#include "Platform/Vulkan/Vk_Buffer.h"
#include "Platform/Vulkan/Vk_DescriptorSet.h"
#include "Platform/Vulkan/Vk_CommandBuffer.h"

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
        void createTextureImage();
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void createBuffers();
        void createUniformBuffers();
        void createDescriptorSets();
        void createCommandBuffers();
        void createSyncObjects();
        void recreateSwapChain();
        void updateUniformBuffer(uint32_t currentImage);


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
        Graphics::YzVkInstance m_YzInstance;
        Graphics::YzVkDevice* m_YzDevice;
        Graphics::YzVkSwapchain m_YzSwapchain;
        Graphics::YzVkRenderPass m_YzRenderPass;
        Graphics::YzVkPipeline m_YzPipeline;
        std::vector<Graphics::YzVkFramebuffer> m_YzFramebuffers;
        Graphics::YzVkDescriptorSet m_YzDescriptorSets;
        Graphics::YzVkBuffer m_VertexBuffer;
        Graphics::YzVkBuffer m_IndexBuffer;
        std::vector<Graphics::YzVkBuffer> m_UniformBuffers;
        std::vector<Graphics::YzVkCommandBuffer> m_YzCommandBuffers;


        VkImage m_TextureImage;
        VkDeviceMemory m_TextureImageMemory;
        std::vector<VkSemaphore> m_ImageAvailableSemaphore;
        std::vector<VkSemaphore> m_RenderFinishedSemaphore;
        size_t m_CurrentFrame = 0;

    };
}



#endif //YAREZO_VULKAN_H
