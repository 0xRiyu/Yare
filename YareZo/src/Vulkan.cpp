//
// Created by Drew on 11/6/2019.
//

#include <iostream>
#include <stdexcept>
#include <set>
#include <cstdint>
#include <algorithm>

#include <stb_image.h>
#include <tiny_obj_loader.h>

#include "Utilities/YzLogger.h"
#include "Utilities/IOHelper.h"

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_Shader.h"
#include "Platform/Vulkan/Vk_Utilities.h"
#include "src/Vulkan.h"
#include "Window.h"
#include "src/Application.h"


namespace Yarezo {


    GraphicsDevice_Vulkan::GraphicsDevice_Vulkan()
        :m_ImageAvailableSemaphores(MAX_FRAMES_IN_FLIGHT),
         m_RenderFinishedSemaphores(MAX_FRAMES_IN_FLIGHT) {

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

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            m_ImageAvailableSemaphores[i].cleanUp();
            m_RenderFinishedSemaphores[i].cleanUp();
        }

        // Cleans up command pool
        m_YzInstance.cleanUp();

        Graphics::YzVkDevice::release();
    }

    void GraphicsDevice_Vulkan::cleanupSwapChain() {
        
        m_DepthBuffer->cleanUp();
        delete m_DepthBuffer;

        for (int i = m_YzFramebuffers.size() - 1; i >= 0; i--) {
            m_YzFramebuffers[i].cleanUp();
            m_YzFramebuffers.pop_back();
        }

        for (Graphics::YzVkCommandBuffer& commandBuffer : m_YzCommandBuffers) {
            commandBuffer.cleanUp();
        }

        m_YzPipeline.cleanUp();
        m_YzRenderPass.cleanUp();
        m_YzSwapchain.cleanUp();

        for (size_t i = 0; i < m_YzSwapchain.getImagesSize(); i++) {
            m_UniformBuffers[i].cleanUp();
        }

    }

    void GraphicsDevice_Vulkan::createGraphicsPipeline() {

        Graphics::YzVkShader shader("..\\..\\..\\..\\YareZo\\Resources\\Shaders", "texture.shader");

        Graphics::PipelineInfo pipelineInfo = { &shader,  &m_YzRenderPass, &m_YzSwapchain };
        m_YzPipeline.init(pipelineInfo);
    }


    void GraphicsDevice_Vulkan::initVulkan() {

        m_YzInstance.init();
        // Create our static device singleton
        m_YzDevice = Graphics::YzVkDevice::instance();
        // Create a swapchain, a swapchain is responsible for maintaining the images
        // that will be presented to the user. 
        m_YzSwapchain.init();
        // Create the Renderpass, the render pass is responsible for the draw calls.
        // It creates a description/map of a graphics job.
        Graphics::RenderPassInfo renderPassInfo{ m_YzSwapchain.getImageFormat() };
        m_YzRenderPass.init(renderPassInfo);
        // Graphics Pipeline
        createGraphicsPipeline();
        createDepthResources();
        // Frame Buffers
        createFramebuffers();
        // Create a command pool which will manage the memory to store command buffers.
        m_YzInstance.createCommandPool();
        // Texture stuff
        m_TextureImage = Graphics::YzVkImage::createTexture2D( "..\\..\\..\\..\\YareZo\\Resources\\Textures\\chalet.jpg");
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
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            m_ImageAvailableSemaphores[i].init();
            m_RenderFinishedSemaphores[i].init();
        }
    }

    void GraphicsDevice_Vulkan::drawFrame(double deltaTime) {
        m_DeltaTime = deltaTime;
        VkResult result = m_YzSwapchain.acquireNextImage(m_ImageAvailableSemaphores[m_CurrentFrame].getSemaphore());

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS) {
            YZ_ERROR("Vulkan failed to aquire a swapchain image.");
        }

        updateUniformBuffer(m_YzSwapchain.getCurrentImage());

        m_YzCommandBuffers[m_YzSwapchain.getCurrentImage()].submitGfxQueue(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                                                           m_ImageAvailableSemaphores[m_CurrentFrame].getSemaphore(),
                                                                           m_RenderFinishedSemaphores[m_CurrentFrame].getSemaphore(),
                                                                           true);

        result = m_YzSwapchain.present(m_RenderFinishedSemaphores[m_CurrentFrame].getSemaphore());

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) {
            YZ_ERROR("Vulkan failed to present a swapchain image.");
        }

        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void GraphicsDevice_Vulkan::waitIdle() {
        m_YzDevice->waitIdle();
    }

    void GraphicsDevice_Vulkan::createFramebuffers() {
        Graphics::FramebufferInfo framebufferInfo;
        framebufferInfo.type = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = &m_YzRenderPass;
        framebufferInfo.width = m_YzSwapchain.getExtent().width;
        framebufferInfo.height = m_YzSwapchain.getExtent().height;
        framebufferInfo.layers = 1;

        for (uint32_t i = 0; i < m_YzSwapchain.getImageViewSize(); i++) {
            framebufferInfo.attachments = { m_YzSwapchain.getImageView(i), m_DepthBuffer->m_ImageView };
             m_YzFramebuffers.emplace_back(framebufferInfo);
        }
    }

    void GraphicsDevice_Vulkan::createDepthResources() {

        VkFormat depthFormat = Graphics::VkUtil::findDepthFormat();
        m_DepthBuffer = Graphics::YzVkImage::createDepthStencilBuffer(depthFormat, m_YzSwapchain.getExtent().width, m_YzSwapchain.getExtent().height);
    }



    void GraphicsDevice_Vulkan::createBuffers() {
        Utilities::loadModel("..\\..\\..\\..\\YareZo\\Resources\\Models\\chalet.obj", m_Vertices, m_Indices);

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
        size_t swapchainImagesSize = m_YzSwapchain.getImagesSize();
        m_UniformBuffers.resize(swapchainImagesSize);

        for (size_t i = 0; i < swapchainImagesSize; i++) {
            m_UniformBuffers[i].init(usageFlags, (size_t)bufferSize, nullptr);
        }
    }

    void GraphicsDevice_Vulkan::createDescriptorSets() {
        size_t swapchainImagesSize = m_YzSwapchain.getImagesSize();

        Graphics::DescriptorSetInfo descriptorSetInfo;
        descriptorSetInfo.descriptorSetCount = static_cast<uint32_t>(swapchainImagesSize);
        descriptorSetInfo.pipeline = &m_YzPipeline;

        // First create the descriptor set, but the buffers are empty
        m_YzDescriptorSets.init(descriptorSetInfo);

        // For each of our swapchain images, load the associated buffer into the descriptor set
        for (size_t i = 0; i < swapchainImagesSize; i++) {
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

        for (size_t i = 0; i < m_YzCommandBuffers.size(); i++) {

            m_YzCommandBuffers[i].init();

            m_YzCommandBuffers[i].beginRecording();

            m_YzRenderPass.beginRenderPass(&m_YzCommandBuffers[i], &m_YzFramebuffers[i], &m_YzSwapchain);

            m_YzPipeline.setActive(m_YzCommandBuffers[i]);

            m_VertexBuffer.bind(m_YzCommandBuffers[i]);
            m_IndexBuffer.bind(m_YzCommandBuffers[i]);

            vkCmdBindDescriptorSets(m_YzCommandBuffers[i].getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_YzPipeline.getPipelineLayout(), 0, 1, &m_YzDescriptorSets.getDescriptorSet(i), 0, nullptr);
            vkCmdDrawIndexed(m_YzCommandBuffers[i].getCommandBuffer(), static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);

            m_YzRenderPass.endRenderPass(&m_YzCommandBuffers[i]);

            m_YzCommandBuffers[i].endRecording();
        }
    }

    void GraphicsDevice_Vulkan::recreateSwapChain() {
        int width = 0;
        int height = 0;
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::getAppInstance()->getWindow()->getNativeWindow());

        glfwGetFramebufferSize(window, &width, &height);

        if (width == 0 || height == 0) {
            YZ_INFO("Application was minimized.");
            while (width == 0 || height == 0) {
                glfwGetFramebufferSize(window, &width, &height);
                glfwWaitEvents();
            }
            YZ_INFO("Application is no longer minimized.");
        }
        YZ_INFO("The application window has been re-sized, the new dimensions [W,H]  are: " + std::to_string(width) + ", " + std::to_string(height));

        m_YzDevice->waitIdle();
        cleanupSwapChain();
        m_YzSwapchain.init();
        Graphics::RenderPassInfo renderPassInfo{ m_YzSwapchain.getImageFormat() };
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
