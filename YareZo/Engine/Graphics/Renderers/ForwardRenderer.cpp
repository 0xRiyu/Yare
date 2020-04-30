#include "Graphics/Renderers/ForwardRenderer.h"

#include "Platform/Vulkan/Vk_Utilities.h"

#include "Application/Application.h"

namespace Yarezo::Graphics {

    ForwardRenderer::ForwardRenderer() {
        init();
    }

    ForwardRenderer::~ForwardRenderer() {
        cleanupSwapChain();

        m_DefaultTextureImage->cleanUp();
        delete m_DefaultTextureImage;
        m_TextureImage->cleanUp();
        delete m_TextureImage;

        delete m_ChaletModel;

        delete m_Renderer;
    }

    void ForwardRenderer::cleanupSwapChain() {

        m_DepthBuffer->cleanUp();
        delete m_DepthBuffer;

        for (int i = (int)m_FrameBuffers.size() - 1; i >= 0; i--) {
            m_FrameBuffers[i]->cleanUp();
            delete m_FrameBuffers[i];
            m_FrameBuffers.pop_back();
        }

        for (auto commandBuffer : m_CommandBuffers) {
            commandBuffer->cleanUp();
            delete commandBuffer;
        }

        m_Pipeline->cleanUp();
        delete m_Pipeline;

        m_RenderPass->cleanUp();
        delete m_RenderPass;

        for (size_t i = 0; i < m_Renderer->getYzSwapchain()->getImagesSize(); i++) {
            m_UniformBuffers[i]->cleanUp();
            delete m_UniformBuffers[i];
        }
    }

    void ForwardRenderer::init() {
        m_Renderer = new YzVkRenderer();
        m_Renderer->init();

        m_WindowWidth =  m_Renderer->getYzSwapchain()->getExtent().width;
        m_WindowHeight = m_Renderer->getYzSwapchain()->getExtent().height;

        const RenderPassInfo renderPassInfo{ m_Renderer->getYzSwapchain()->getImageFormat() };
        m_RenderPass = new YzVkRenderPass();
        m_RenderPass->init(renderPassInfo);

        createGraphicsPipeline();

        createFrameBuffers();

        // Uniform Buffers
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        size_t swapchainImagesSize = m_Renderer->getYzSwapchain()->getImagesSize();
        m_UniformBuffers.resize(swapchainImagesSize);

        for (size_t i = 0; i < swapchainImagesSize; i++) {
            m_UniformBuffers[i] = new YzVkBuffer();
            m_UniformBuffers[i]->init(usageFlags, (size_t)bufferSize, nullptr);
        }

        createDescriptorSets();

        createCommandBuffers();

        m_ChaletModel = new Model("../YareZo/Resources/Models/chalet.obj");
        m_TextureImage = YzVkImage::createTexture2D( "../YareZo/Resources/Textures/chalet.jpg");

    }

    void ForwardRenderer::waitIdle() {
        YzVkDevice::instance()->waitIdle();
    }

    void ForwardRenderer::renderScene() {
        if (!m_Renderer->begin()) {
            recreateSwapChain();
        }

        m_CurrentBufferID = m_Renderer->getYzSwapchain()->getCurrentImage();

        begin();

        //auto cubeModel = new Model("../YareZo/Resources/Models/cube.obj");

        glm::mat4 model_transform = glm::mat4(1.0f);
        model_transform = glm::rotate(model_transform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_transform = glm::rotate(model_transform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        submitModel(m_ChaletModel, model_transform);

        present();

        end();
    }

    void ForwardRenderer::submitModel(Model* model, const glm::mat4& transform) {
        RenderCommand renderCommand;
        renderCommand.model = model;
        renderCommand.texture = m_TextureImage;
        renderCommand.transform = transform;

        m_CommandQueue.push_back(renderCommand);
    }

    void ForwardRenderer::present() {

        for (auto& command : m_CommandQueue) {

            updateUniformBuffer(m_CurrentBufferID);

            YzVkCommandBuffer* currentCommandBuffer = m_CommandBuffers[m_CurrentBufferID];

            m_Pipeline->setActive(*currentCommandBuffer);

            command.model->getMesh()->getVertexBuffer()->bind(*currentCommandBuffer);
            command.model->getMesh()->getIndexBuffer()->bind(*currentCommandBuffer);
            m_ModelPos = command.transform;

            vkCmdBindDescriptorSets(currentCommandBuffer->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->getPipelineLayout(), 0u, 1u, &m_DescriptorSet->getDescriptorSet(m_CurrentBufferID), 0u, nullptr);
            vkCmdDrawIndexed(currentCommandBuffer->getCommandBuffer(), static_cast<uint32_t>(command.model->getMesh()->getIndexBuffer()->getSize() / sizeof(uint32_t)), 1, 0, 0, 0);

        }
    }

    void ForwardRenderer::begin() {
        m_CommandQueue.clear();

        m_CommandBuffers[m_CurrentBufferID]->beginRecording();

        m_RenderPass->beginRenderPass(m_CommandBuffers[m_CurrentBufferID], m_FrameBuffers[m_CurrentBufferID], m_Renderer->getYzSwapchain().get());
    }

    void ForwardRenderer::end() {
        m_RenderPass->endRenderPass(m_CommandBuffers[m_CurrentBufferID]);

        m_CommandBuffers[m_CurrentBufferID]->endRecording();

        if (!m_Renderer->present(m_CommandBuffers[m_CurrentBufferID])) {
            recreateSwapChain();
        }
    }

    void ForwardRenderer::recreateSwapChain() {
        m_WindowWidth =  m_Renderer->getYzSwapchain()->getExtent().width;
        m_WindowHeight = m_Renderer->getYzSwapchain()->getExtent().height;

        cleanupSwapChain();
        RenderPassInfo renderPassInfo{ m_Renderer->getYzSwapchain()->getImageFormat() };
        m_RenderPass = new YzVkRenderPass();
        m_RenderPass->init(renderPassInfo);
        createGraphicsPipeline();
        createFrameBuffers();

        // Uniform Buffers
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        size_t swapchainImagesSize = m_Renderer->getYzSwapchain()->getImagesSize();
        m_UniformBuffers.resize(swapchainImagesSize);

        for (size_t i = 0; i < swapchainImagesSize; i++) {
            m_UniformBuffers[i] = new YzVkBuffer();
            m_UniformBuffers[i]->init(usageFlags, (size_t)bufferSize, nullptr);
        }

        createDescriptorSets();
        createCommandBuffers();
    }

    void ForwardRenderer::createGraphicsPipeline() {
        Graphics::YzVkShader shader("../YareZo/Resources/Shaders", "texture.shader");

        Graphics::PipelineInfo pipelineInfo = { &shader,  m_RenderPass, m_Renderer->getYzSwapchain().get() };
        m_Pipeline = new YzVkPipeline();
        m_Pipeline->init(pipelineInfo);
    }

    void ForwardRenderer::createFrameBuffers() {
        VkFormat depthFormat = VkUtil::findDepthFormat();
        m_DepthBuffer = YzVkImage::createDepthStencilBuffer(depthFormat, m_WindowWidth, m_WindowHeight);

        FramebufferInfo framebufferInfo;
        framebufferInfo.type = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_RenderPass;
        framebufferInfo.width = m_WindowWidth;
        framebufferInfo.height = m_WindowHeight;
        framebufferInfo.layers = 1;

        for (uint32_t i = 0; i < m_Renderer->getYzSwapchain()->getImageViewSize(); i++) {
            framebufferInfo.attachments = { m_Renderer->getYzSwapchain()->getImageView(i), m_DepthBuffer->m_ImageView };
            m_FrameBuffers.push_back(new YzVkFramebuffer(framebufferInfo));
        }
    }

    void ForwardRenderer::createDescriptorSets() {
        size_t swapchainImagesSize = m_Renderer->getYzSwapchain()->getImagesSize();

        Graphics::DescriptorSetInfo descriptorSetInfo;
        descriptorSetInfo.descriptorSetCount = static_cast<uint32_t>(swapchainImagesSize);
        descriptorSetInfo.pipeline = m_Pipeline;

        // First create the descriptor set, but the buffers are empty
        m_DescriptorSet = new YzVkDescriptorSet();
        m_DescriptorSet->init(descriptorSetInfo);

        m_DefaultTextureImage = YzVkImage::createTexture2D( "../YareZo/Resources/Textures/sprite.jpg");

        // For each of our swapchain images, load the associated buffer into the descriptor set
        for (int i = 0; i < swapchainImagesSize; i++) {
            BufferInfo bufferInfo;
            bufferInfo.buffer = m_UniformBuffers[i]->getBuffer();
            bufferInfo.offset = 0;
            bufferInfo.size = sizeof(UniformBufferObject);
            bufferInfo.binding = i;
            bufferInfo.imageSampler = m_DefaultTextureImage->m_YzSampler.getSampler();
            bufferInfo.imageView = m_DefaultTextureImage->m_ImageView;

            m_DescriptorSet->update(bufferInfo);
        }
    }

    void ForwardRenderer::createCommandBuffers() {
        m_CommandBuffers.resize(m_FrameBuffers.size());

        for (unsigned int i = 0; i < m_CommandBuffers.size(); i++) {
            m_CurrentBufferID = i;

            m_CommandBuffers[i] = new YzVkCommandBuffer();
            m_CommandBuffers[i]->init();
        }
    }

    void ForwardRenderer::updateUniformBuffer(uint32_t currentImage) {
        UniformBufferObject ubo = {};
        ubo.model = m_ModelPos;
        ubo.view = Application::getAppInstance()->getWindow()->getCamera()->getViewMatrix();
        ubo.proj = Application::getAppInstance()->getWindow()->getCamera()->getProjectionMatrix();
        ubo.proj[1][1] *= -1;

        m_UniformBuffers[currentImage]->setData(sizeof(ubo), &ubo);
    }

}




