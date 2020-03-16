//
// Created by Drew on 11/6/2019.
//

#include <iostream>
#include <stdexcept>
#include <set>
#include <cstdint>
#include <algorithm>

#include <stb_image.h>

#include "Utilities/YzLogger.h"
#include "Utilities/IOHelper.h"

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_Shader.h"
#include "Platform/Vulkan/Vk_Utilities.h"
#include "src/Vulkan.h"
#include "Window.h"
#include "src/Application.h"


namespace Yarezo {


    GraphicsDevice_Vulkan::GraphicsDevice_Vulkan() {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        m_ModelPos = model;

        initVulkan();
    }

    GraphicsDevice_Vulkan::~GraphicsDevice_Vulkan() {
        cleanupSwapChain();
        
        vkDestroySampler(Graphics::YzVkDevice::instance()->getDevice(), m_TextureSampler, nullptr);
        vkDestroyImageView(Graphics::YzVkDevice::instance()->getDevice(), m_TextureImageView, nullptr);

        vkDestroyImage(Graphics::YzVkDevice::instance()->getDevice(), m_TextureImage, nullptr);
        vkFreeMemory(Graphics::YzVkDevice::instance()->getDevice(), m_TextureImageMemory, nullptr);

        m_YzPipeline.cleanupDescSetLayout();

        m_VertexBuffer.cleanUp();
        m_IndexBuffer.cleanUp();

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(Graphics::YzVkDevice::instance()->getDevice(), m_RenderFinishedSemaphore[i], nullptr);
            vkDestroySemaphore(Graphics::YzVkDevice::instance()->getDevice(), m_ImageAvailableSemaphore[i], nullptr);
        }
        // Cleans up command pool
        m_YzInstance.cleanUp();

        Graphics::YzVkDevice::release();
    }

    void GraphicsDevice_Vulkan::cleanupSwapChain() {
        vkDestroyImageView(Graphics::YzVkDevice::instance()->getDevice(), m_DepthImageView, nullptr);
        vkDestroyImage(Graphics::YzVkDevice::instance()->getDevice(), m_DepthImage, nullptr);
        vkFreeMemory(Graphics::YzVkDevice::instance()->getDevice(), m_DepthImageMemory, nullptr);
        
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
        createTextureImage();
        createTextureImageView();
        createTextureSampler();
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

    void GraphicsDevice_Vulkan::drawFrame(double deltaTime) {
        m_DeltaTime = deltaTime;
        VkResult result = m_YzSwapchain.acquireNextImage(m_ImageAvailableSemaphore[m_CurrentFrame]);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS) {
            YZ_ERROR("Vulkan failed to aquire a swapchain image.");
        }

        updateUniformBuffer(m_YzSwapchain.getCurrentImage());

        m_YzCommandBuffers[m_YzSwapchain.getCurrentImage()].submitGfxQueue(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, m_ImageAvailableSemaphore[m_CurrentFrame], m_RenderFinishedSemaphore[m_CurrentFrame], true);

        result = m_YzSwapchain.present(m_RenderFinishedSemaphore[m_CurrentFrame]);

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
            framebufferInfo.attachments = { m_YzSwapchain.getImageView(i), m_DepthImageView };
             m_YzFramebuffers.emplace_back(framebufferInfo);
        }
    }

    void GraphicsDevice_Vulkan::createTextureImage() {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load("..\\..\\..\\..\\YareZo\\Resources\\Textures\\sprite.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels) {
            YZ_CRITICAL("failed to load texture image");
        }

        Graphics::YzVkBuffer stagingBuffer{VK_BUFFER_USAGE_TRANSFER_SRC_BIT, imageSize, pixels};

        stbi_image_free(pixels);

        createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_TextureImage, m_TextureImageMemory);

        transitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        Graphics::VkUtil::copyBufferToImage(stagingBuffer.getBuffer(), m_TextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        transitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    void GraphicsDevice_Vulkan::createTextureImageView() {
        m_TextureImageView = Graphics::VkUtil::createImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    void GraphicsDevice_Vulkan::createTextureSampler() {
        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 16;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(Graphics::YzVkDevice::instance()->getDevice(), &samplerInfo, nullptr, &m_TextureSampler) != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to create a texture sampler.");
        }

    }

    void GraphicsDevice_Vulkan::createDepthResources() {
        VkFormat depthFormat = Graphics::VkUtil::findDepthFormat();
        createImage(m_YzSwapchain.getExtent().width, m_YzSwapchain.getExtent().height,
                    depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_DepthImage, m_DepthImageMemory);
        m_DepthImageView = Graphics::VkUtil::createImageView(m_DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    }

    void GraphicsDevice_Vulkan::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(Graphics::YzVkDevice::instance()->getDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
            YZ_CRITICAL("Failed to create an image.");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(Graphics::YzVkDevice::instance()->getDevice(), image, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = Graphics::VkUtil::findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (vkAllocateMemory(Graphics::YzVkDevice::instance()->getDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            YZ_CRITICAL("failed to allocate image memory!");
        }

        if (vkBindImageMemory(Graphics::YzVkDevice::instance()->getDevice(), image, imageMemory, 0) != VK_SUCCESS) {
            YZ_ERROR("Failed to bind image memory");
        }
    }

    void GraphicsDevice_Vulkan::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
        VkCommandBuffer commandBuffer = Graphics::VkUtil::beginSingleTimeCommands();

        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else {
            YZ_CRITICAL("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        Graphics::VkUtil::endSingleTimeCommands(commandBuffer);
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
            bufferInfo.imageSampler = m_TextureSampler;
            bufferInfo.imageView = m_TextureImageView;

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
            vkCmdDrawIndexed(m_YzCommandBuffers[i].getCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

            m_YzRenderPass.endRenderPass(&m_YzCommandBuffers[i]);

            m_YzCommandBuffers[i].endRecording();
        }
    }

    void GraphicsDevice_Vulkan::createSyncObjects() {
        m_ImageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(Graphics::YzVkDevice::instance()->getDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore[i]) != VK_SUCCESS ||
                vkCreateSemaphore(Graphics::YzVkDevice::instance()->getDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore[i]) != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan failed to create sync objects. (Semaphores)");
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
        createDepthResources();
        createFramebuffers();
        createUniformBuffers();
        createDescriptorSets();
        createCommandBuffers();
    }

    void GraphicsDevice_Vulkan::updateUniformBuffer(uint32_t currentImage) {

        auto rotationAmount = m_DeltaTime * glm::radians(90.0f);
        m_ModelPos = glm::rotate(m_ModelPos, float(rotationAmount * m_RotationDir), glm::vec3(0.0f, 0.0f, 1.0f));

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
