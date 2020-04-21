//
// Created by Drew on 11/6/2019.
//
#include "Graphics/Vulkan.h"

#include <iostream>
#include <stdexcept>
#include <set>
#include <cstdint>
#include <algorithm>

#include <stb/stb_image.h>
#include <tinyobjloader/tiny_obj_loader.h>

#include "Utilities/IOHelper.h"

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_Shader.h"
#include "Platform/Vulkan/Vk_Utilities.h"
#include "Core/Yzh.h"

namespace Yarezo {


    GraphicsDevice_Vulkan::GraphicsDevice_Vulkan() {

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        m_ModelPos = model;

        initVulkan();
    }

    GraphicsDevice_Vulkan::~GraphicsDevice_Vulkan() {
        cleanupSwapChain();

        m_TextureImage->cleanUp();
        delete m_TextureImage;

        m_YzPipeline.cleanupDescSetLayout();

        m_VertexBuffer.cleanUp();
        m_IndexBuffer.cleanUp();

        //Cleans up command pool
        m_YzInstance.cleanUp();

        m_YzRenderer.reset();

        Graphics::YzVkDevice::release();
    }

    void GraphicsDevice_Vulkan::cleanupSwapChain() {

         m_DepthBuffer->cleanUp();
         delete m_DepthBuffer;

         for (int i = (int)m_YzFramebuffers.size() - 1; i >= 0; i--) {
             m_YzFramebuffers[i].cleanUp();
             m_YzFramebuffers.pop_back();
        }

        for (Graphics::YzVkCommandBuffer& commandBuffer : m_YzCommandBuffers) {
            commandBuffer.cleanUp();
        }

        m_YzPipeline.cleanUp();
        m_YzRenderPass.cleanUp();

        for (size_t i = 0; i < m_YzRenderer->getYzSwapchain()->getImagesSize(); i++) {
            m_UniformBuffers[i].cleanUp();
        }

    }

    void GraphicsDevice_Vulkan::createGraphicsPipeline() {

        Graphics::YzVkShader shader("..\\YareZo\\Resources\\Shaders", "texture.shader");

        Graphics::PipelineInfo pipelineInfo = { &shader,  &m_YzRenderPass, m_YzRenderer->getYzSwapchain().get() };
        m_YzPipeline.init(pipelineInfo);
    }


    void GraphicsDevice_Vulkan::initVulkan() {

        m_YzInstance.init();
        // Create our static device singleton
        m_YzDevice = Graphics::YzVkDevice::instance();
        m_YzRenderer = std::make_unique<Graphics::YzVkRenderer>();
        m_YzRenderer->init();
        // Create the Renderpass, the render pass is responsible for the draw calls.
        // It creates a description/map of a graphics job.
        Graphics::RenderPassInfo renderPassInfo{ m_YzRenderer->getYzSwapchain()->getImageFormat() };
        m_YzRenderPass.init(renderPassInfo);
        // Graphics Pipeline
        createGraphicsPipeline();
        createDepthResources();
        // Frame Buffers
        createFramebuffers();
        // Create a command pool which will manage the memory to store command buffers.
        m_YzInstance.createCommandPool();
        // Texture stuff
        m_TextureImage = Graphics::YzVkImage::createTexture2D( "..\\YareZo\\Resources\\Textures\\chalet.jpg");
        // Create the Vertex/Indices/Uniform buffers;
        // A vertex data will store arbitrary triangle data to be read by the GPU
        // The indices data will connect the vertices data suc that we can re-use some vertices
        // instead of redefining them
        // The uniform buffers are for storing the projection matrices
        createBuffers();
        createUniformBuffers();
        // Create a set of descriptors for the shader to receive,
        // A descriptor set is called a "set" because it can refer to an array of homogenous resources that can be described with the same layout binding. 
        createDescriptorSets();
        // Create a command buffer which will record commands that are submitted for execution on the GPU
        createCommandBuffers();
    }

    void GraphicsDevice_Vulkan::drawFrame(double deltaTime) {
        m_DeltaTime = deltaTime;

        if (!m_YzRenderer->begin()) {
            recreateSwapChain();
        }

        uint32_t currentimage = m_YzRenderer->getYzSwapchain()->getCurrentImage();

        updateUniformBuffer(currentimage);

        if (!m_YzRenderer->present(&m_YzCommandBuffers[currentimage])) {
            recreateSwapChain();
        }

    }

    void GraphicsDevice_Vulkan::waitIdle() {
        m_YzDevice->waitIdle();
    }

    void GraphicsDevice_Vulkan::createFramebuffers() {
        Graphics::FramebufferInfo framebufferInfo;
        framebufferInfo.type = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = &m_YzRenderPass;
        framebufferInfo.width = m_YzRenderer->getYzSwapchain()->getExtent().width;
        framebufferInfo.height = m_YzRenderer->getYzSwapchain()->getExtent().height;
        framebufferInfo.layers = 1;

        for (uint32_t i = 0; i < m_YzRenderer->getYzSwapchain()->getImageViewSize(); i++) {
            framebufferInfo.attachments = { m_YzRenderer->getYzSwapchain()->getImageView(i), m_DepthBuffer->m_ImageView };
             m_YzFramebuffers.emplace_back(framebufferInfo);
        }
    }

    void GraphicsDevice_Vulkan::createDepthResources() {

        VkFormat depthFormat = Graphics::VkUtil::findDepthFormat();
        m_DepthBuffer = Graphics::YzVkImage::createDepthStencilBuffer(depthFormat, m_YzRenderer->getYzSwapchain()->getExtent().width, m_YzRenderer->getYzSwapchain()->getExtent().height);
    }



    void GraphicsDevice_Vulkan::createBuffers() {
        Utilities::loadModel("..\\YareZo\\Resources\\Models\\chalet.obj", m_Vertices, m_Indices);

        // Vertex Buffers
        VkDeviceSize bufferSize = sizeof(Vertex) * m_Vertices.size();
        VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        m_VertexBuffer.init(usageFlags, (size_t)bufferSize, m_Vertices.data());

        // Index Buffers
        bufferSize = sizeof(m_Indices[0]) * m_Indices.size();
        usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

        m_IndexBuffer.init(usageFlags, (size_t)bufferSize, m_Indices.data());
    }

    void GraphicsDevice_Vulkan::createUniformBuffers() {
        // Uniform Buffers
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        size_t swapchainImagesSize = m_YzRenderer->getYzSwapchain()->getImagesSize();
        m_UniformBuffers.resize(swapchainImagesSize);

        for (size_t i = 0; i < swapchainImagesSize; i++) {
            m_UniformBuffers[i].init(usageFlags, (size_t)bufferSize, nullptr);
        }
    }

    void GraphicsDevice_Vulkan::createDescriptorSets() {
        size_t swapchainImagesSize = m_YzRenderer->getYzSwapchain()->getImagesSize();

        Graphics::DescriptorSetInfo descriptorSetInfo;
        descriptorSetInfo.descriptorSetCount = static_cast<uint32_t>(swapchainImagesSize);
        descriptorSetInfo.pipeline = &m_YzPipeline;

        // First create the descriptor set, but the buffers are empty
        m_YzDescriptorSets.init(descriptorSetInfo);

        // For each of our swapchain images, load the associated buffer into the descriptor set
        for (int i = 0; i < swapchainImagesSize; i++) {
            Graphics::BufferInfo bufferInfo;
            bufferInfo.buffer = m_UniformBuffers[i].getBuffer();
            bufferInfo.offset = 0;
            bufferInfo.size = sizeof(UniformBufferObject);
            bufferInfo.binding = i;
            bufferInfo.imageSampler = m_TextureImage->m_YzSampler.getSampler();
            bufferInfo.imageView = m_TextureImage->m_ImageView;

            m_YzDescriptorSets.update(bufferInfo);
        }
    }

    void GraphicsDevice_Vulkan::createCommandBuffers() {
        m_YzCommandBuffers.resize(m_YzFramebuffers.size());

        for (unsigned int i = 0; i < m_YzCommandBuffers.size(); i++) {

            m_YzCommandBuffers[i].init();

            m_YzCommandBuffers[i].beginRecording();

            m_YzRenderPass.beginRenderPass(&m_YzCommandBuffers[i], &m_YzFramebuffers[i], m_YzRenderer->getYzSwapchain().get());

            m_YzPipeline.setActive(m_YzCommandBuffers[i]);

            m_VertexBuffer.bind(m_YzCommandBuffers[i]);
            m_IndexBuffer.bind(m_YzCommandBuffers[i]);

            vkCmdBindDescriptorSets(m_YzCommandBuffers[i].getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_YzPipeline.getPipelineLayout(), 0u, 1u, &m_YzDescriptorSets.getDescriptorSet(i), 0u, nullptr);
            vkCmdDrawIndexed(m_YzCommandBuffers[i].getCommandBuffer(), static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);

            m_YzRenderPass.endRenderPass(&m_YzCommandBuffers[i]);

            m_YzCommandBuffers[i].endRecording();
        }
    }

    void GraphicsDevice_Vulkan::recreateSwapChain() {
        cleanupSwapChain();
        Graphics::RenderPassInfo renderPassInfo{ m_YzRenderer->getYzSwapchain()->getImageFormat() };
        m_YzRenderPass.init(renderPassInfo);
        createGraphicsPipeline();
        createDepthResources();
        createFramebuffers();
        createUniformBuffers();
        createDescriptorSets();
        createCommandBuffers();
    }

    void GraphicsDevice_Vulkan::updateUniformBuffer(uint32_t currentImage) {

        auto rotationAmount = m_DeltaTime * glm::radians(90.0f);
        //m_ModelPos = glm::rotate(m_ModelPos, float(rotationAmount * m_RotationDir), glm::vec3(0.0f, 0.0f, 1.0f));

        if (m_ModelPos[0][1] > 0.4f) {
            m_RotationDir = 1;
        }
        else if (m_ModelPos[0][1] < -0.4f) {
            m_RotationDir = -1;
        }

        UniformBufferObject ubo = {};
        ubo.model = m_ModelPos;
        ubo.view = Application::getAppInstance()->getWindow()->getCamera()->getViewMatrix();
        ubo.proj = Application::getAppInstance()->getWindow()->getCamera()->getProjectionMatrix();
        ubo.proj[1][1] *= -1;

        m_UniformBuffers[currentImage].setData(sizeof(ubo), &ubo);
    }
}
