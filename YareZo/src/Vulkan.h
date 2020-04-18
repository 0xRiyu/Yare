//
// Created by Drew on 11/6/2019.
//
#ifndef YAREZO_VULKAN_H
#define YAREZO_VULKAN_H
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

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
#include "Platform/Vulkan/Vk_Semaphore.h"
#include "Platform/Vulkan/Vk_Image.h"
#include "Platform/Vulkan/Vk_Renderer.h"

namespace Yarezo {

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };


    class GraphicsDevice_Vulkan {
    public:
        GraphicsDevice_Vulkan();

        virtual ~GraphicsDevice_Vulkan();

        void initVulkan();
        void drawFrame(double deltaTime);
        void waitIdle();

    private:
        void cleanupSwapChain();
        void createGraphicsPipeline();
        void createFramebuffers();
        void createDepthResources();
        void createBuffers();
        void createUniformBuffers();
        void createDescriptorSets();
        void createCommandBuffers();
        void recreateSwapChain();
        void updateUniformBuffer(uint32_t currentImage);



        int m_RotationDir = 1;
        double m_DeltaTime = 0;
        glm::mat4 m_ModelPos;

        // Class members created by GraphicsDevice_Vulkan class
        Graphics::YzVkInstance                      m_YzInstance;
        Graphics::YzVkDevice*                       m_YzDevice;
        Graphics::YzVkRenderPass                    m_YzRenderPass;
        Graphics::YzVkPipeline                      m_YzPipeline;
        std::unique_ptr<Graphics::YzVkRenderer>     m_YzRenderer;
        std::vector<Graphics::YzVkFramebuffer>      m_YzFramebuffers;
        std::vector<Graphics::YzVkBuffer>           m_UniformBuffers;
        std::vector<Graphics::YzVkCommandBuffer>    m_YzCommandBuffers;
        Graphics::YzVkDescriptorSet m_YzDescriptorSets;


        std::vector<Vertex> m_Vertices = {};
        std::vector<uint32_t> m_Indices = {};

        Graphics::YzVkBuffer m_VertexBuffer;
        Graphics::YzVkBuffer m_IndexBuffer;

        Graphics::YzVkImage* m_TextureImage;
        Graphics::YzVkImage* m_DepthBuffer;


    };
}



#endif //YAREZO_VULKAN_H
