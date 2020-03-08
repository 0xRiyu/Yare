//
// Created by Drew on 11/6/2019.
//

#include <iostream>
#include <stdexcept>
#include <set>
#include <cstdint>
#include <algorithm>

#include "Utilities/YzLogger.h"
#include "Utilities/IOHelper.h"

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_Shader.h"
#include "src/Vulkan.h"
#include "Window.h"
#include "src/Application.h"


namespace Yarezo {


    GraphicsDevice_Vulkan::GraphicsDevice_Vulkan() {
        initVulkan();
    }

    GraphicsDevice_Vulkan::~GraphicsDevice_Vulkan() {
        cleanupSwapChain();
        m_YzPipeline.cleanupDescSetLayout();

        m_VertexBuffer.cleanUp();
        m_IndexBuffer.cleanUp();

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(m_YzDevice->getDevice(), m_RenderFinishedSemaphore[i], nullptr);
            vkDestroySemaphore(m_YzDevice->getDevice(), m_ImageAvailableSemaphore[i], nullptr);
            vkDestroyFence(m_YzDevice->getDevice(), m_InFlightFences[i], nullptr);
        }

        // Cleans up command pool
        m_YzInstance.cleanUp();

        Graphics::YzVkDevice::release();
    }

    void GraphicsDevice_Vulkan::cleanupSwapChain() {

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

        Graphics::YzVkShader shader("..\\..\\..\\..\\YareZo\\Shaders", "ubo.shader");

        Graphics::PipelineInfo pipelineInfo = { &shader,  &m_YzRenderPass, &m_YzSwapchain };
        m_YzPipeline.init(pipelineInfo);

        shader.unloadModules();
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
        // Frame Buffers
        createFramebuffers();
        // Create a command pool which will manage the memory to store command buffers.
        m_YzInstance.createCommandPool();
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
        // Create some semophores/fences to manage workloads in flight to the gpu
        createSyncObjects();
    }

    void GraphicsDevice_Vulkan::drawFrame() {
        vkWaitForFences(m_YzDevice->getDevice(), 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(m_YzDevice->getDevice(), m_YzSwapchain.getSwapchain(), UINT64_MAX, m_ImageAvailableSemaphore[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

        auto window = Application::getAppInstance()->getWindow();

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->windowResized) {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            YZ_ERROR("Vulkan failed to aquire a swapchain image.");
        }

        updateUniformBuffer(imageIndex);

        if (m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(m_YzDevice->getDevice(), 1, &m_ImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }

        m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphore[m_CurrentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_YzCommandBuffers[imageIndex].getCommandBuffer();

        VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphore[m_CurrentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(m_YzDevice->getDevice(), 1, &m_InFlightFences[m_CurrentFrame]);
        if (vkQueueSubmit(m_YzDevice->getGraphicsQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS) {
            YZ_ERROR("Vulkan failed to submit draw command buffer.");
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_YzSwapchain.getSwapchain() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional
        result = vkQueuePresentKHR(m_YzDevice->getPresentQueue(), &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) {
            YZ_ERROR("Vulkan failed to present a swap chain image.");
        }

        vkQueueWaitIdle(m_YzDevice->getPresentQueue());

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
            framebufferInfo.attachments = { m_YzSwapchain.getImageView(i) };
             m_YzFramebuffers.emplace_back(framebufferInfo);
        }
    }

    void GraphicsDevice_Vulkan::createBuffers() {

        // Vertex Buffers
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
        VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        m_VertexBuffer.init(usageFlags, (size_t)bufferSize, vertices.data());

        // Index Buffers
        bufferSize = sizeof(indices[0]) * indices.size();
        usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

        m_IndexBuffer.init(usageFlags, (size_t)bufferSize, indices.data());
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

            m_YzDescriptorSets.Update(bufferInfo);
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
            vkCmdDrawIndexed(m_YzCommandBuffers[i].getCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

            m_YzRenderPass.endRenderPass(&m_YzCommandBuffers[i]);

            m_YzCommandBuffers[i].endRecording();
        }
    }

    void GraphicsDevice_Vulkan::createSyncObjects() {
        m_ImageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
        m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        m_ImagesInFlight.resize(m_YzSwapchain.getImagesSize(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(m_YzDevice->getDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_YzDevice->getDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore[i]) != VK_SUCCESS ||
                vkCreateFence(m_YzDevice->getDevice(), &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan failed to create sync objects. (Semaphores or Fence)");
            }
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
        createFramebuffers();
        createUniformBuffers();
        createDescriptorSets();
        createCommandBuffers();
    }

    void GraphicsDevice_Vulkan::updateUniformBuffer(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo = {};
        glm::mat4 model = glm::mat4(0.5f);
        model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        ubo.model = glm::rotate(model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        ubo.view = Application::getAppInstance()->getWindow()->getCamera()->getViewMatrix();

        ubo.proj = Application::getAppInstance()->getWindow()->getCamera()->getProjectionMatrix();

        ubo.proj[1][1] *= -1;

        m_UniformBuffers[currentImage].setData(sizeof(ubo), &ubo);
    }
}