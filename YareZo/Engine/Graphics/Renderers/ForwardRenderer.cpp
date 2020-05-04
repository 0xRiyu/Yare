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

        //delete m_ChaletModel;
        delete m_VikingModel;
        delete m_CubeModel;

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

        delete m_UniformBuffers.view;
        delete m_UniformBuffers.dynamic;

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

        prepareUniformBuffers();

        createDescriptorSets();

        createCommandBuffers();

        // m_ChaletModel = new Model("../YareZo/Resources/Models/chalet.obj");
        m_VikingModel = new Model("../YareZo/Resources/Models/viking_room.obj");
        m_CubeModel = new Model("../YareZo/Resources/Models/cube.obj");

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

        glm::mat4 model_transform = glm::mat4(1.0f);
        //model_transform = glm::rotate(model_transform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_transform = glm::rotate(model_transform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        submitModel(m_VikingModel, model_transform);

        glm::mat4 model_transform2 = glm::translate(model_transform, glm::vec3(0.0f, 3.0f, 0.0f));
        // submitModel(m_ChaletModel, model_transform2);

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
        int index = 0;
        for (auto& command : m_CommandQueue) {

            m_ModelPos = command.transform;
            updateUniformBuffer(m_CurrentBufferID);

            YzVkCommandBuffer* currentCommandBuffer = m_CommandBuffers[m_CurrentBufferID];

            uint32_t dynamicOffset = index * static_cast<uint32_t>(m_DynamicAlignment);

            m_Pipeline->setActive(*currentCommandBuffer);
            vkCmdBindDescriptorSets(currentCommandBuffer->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->getPipelineLayout(), 0u, 1u, &m_DescriptorSet->getDescriptorSet(0), 1, &dynamicOffset);

            command.model->getMesh()->getVertexBuffer()->bind(*currentCommandBuffer);
            command.model->getMesh()->getIndexBuffer()->bind(*currentCommandBuffer);

            vkCmdDrawIndexed(currentCommandBuffer->getCommandBuffer(), static_cast<uint32_t>(command.model->getMesh()->getIndexBuffer()->getSize() / sizeof(uint32_t)), 1, 0, 0, 0);
            index++;
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
        prepareUniformBuffers();
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
        descriptorSetInfo.descriptorSetCount = 1;
        descriptorSetInfo.pipeline = m_Pipeline;

        // First create the descriptor set, but the buffers are empty
        m_DescriptorSet = new YzVkDescriptorSet();
        m_DescriptorSet->init(descriptorSetInfo);

        m_DefaultTextureImage = YzVkImage::createTexture2D( "../YareZo/Resources/Textures/viking_room.png");

        std::vector<BufferInfo> bufferInfos = {};
        BufferInfo viewBufferInfo = {};
        viewBufferInfo.buffer = m_UniformBuffers.view->getBuffer();
        viewBufferInfo.offset = 0;
        viewBufferInfo.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        viewBufferInfo.size = sizeof(UniformVS);
        viewBufferInfo.binding = 0;
        viewBufferInfo.imageSampler = nullptr;
        viewBufferInfo.imageView = nullptr;

        BufferInfo dynamicBufferInfo = {};
        dynamicBufferInfo.buffer = m_UniformBuffers.dynamic->getBuffer();
        dynamicBufferInfo.offset = 0;
        dynamicBufferInfo.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        dynamicBufferInfo.size = sizeof(glm::mat4);
        dynamicBufferInfo.binding = 1;
        dynamicBufferInfo.imageSampler = nullptr;
        dynamicBufferInfo.imageView = nullptr;

        BufferInfo imageBufferInfo = {};
        imageBufferInfo.buffer = nullptr;
        imageBufferInfo.offset = 0;
        imageBufferInfo.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        imageBufferInfo.size = 0;
        imageBufferInfo.binding = 2;
        imageBufferInfo.imageSampler = m_DefaultTextureImage->m_YzSampler.getSampler();
        imageBufferInfo.imageView = m_DefaultTextureImage->m_ImageView;

        bufferInfos.push_back(viewBufferInfo);
        bufferInfos.push_back(dynamicBufferInfo);
        bufferInfos.push_back(imageBufferInfo);

        m_DescriptorSet->update(bufferInfos);
    }

    void ForwardRenderer::createCommandBuffers() {
        m_CommandBuffers.resize(m_FrameBuffers.size());

        for (unsigned int i = 0; i < m_CommandBuffers.size(); i++) {
            m_CurrentBufferID = i;

            m_CommandBuffers[i] = new YzVkCommandBuffer();
            m_CommandBuffers[i]->init();
        }
    }

    void ForwardRenderer::prepareUniformBuffers() {

        VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

        VkDeviceSize viewBufferSize = sizeof(UniformVS);
        VkMemoryPropertyFlags viewPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        m_DynamicAlignment = sizeof(glm::mat4);
        size_t minUboAlignment = YzVkDevice::instance()->getGPUProperties().limits.minUniformBufferOffsetAlignment;
        if (minUboAlignment > 0) {
            m_DynamicAlignment = (m_DynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
        }

        VkDeviceSize dynamicBufferSize = /* MAX_OBJECTS * */m_DynamicAlignment;
        VkMemoryPropertyFlags dynamicPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

        m_UniformBuffers.view = new YzVkBuffer(usageFlags, viewPropertyFlags, viewBufferSize, nullptr);
        m_UniformBuffers.dynamic = new YzVkBuffer(usageFlags, dynamicPropertyFlags, dynamicBufferSize, nullptr);
    }

    void ForwardRenderer::updateUniformBuffer(uint32_t currentImage) {
        // TODO, store UBOs for each model we want to display in one UBO, separated by an offset
        // then bind based on that offset in the present call
        UboDataDynamic uboDynamic = {};
        uboDynamic.model = &m_ModelPos;
        m_UniformBuffers.dynamic->setDynamicData(m_DynamicAlignment, uboDynamic.model);

        UniformVS uboVS = {};
        uboVS.view = Application::getAppInstance()->getWindow()->getCamera()->getViewMatrix();
        uboVS.projection = Application::getAppInstance()->getWindow()->getCamera()->getProjectionMatrix();
        uboVS.projection[1][1] *= -1;

        m_UniformBuffers.view->setData(sizeof(uboVS), &uboVS);
    }

}




