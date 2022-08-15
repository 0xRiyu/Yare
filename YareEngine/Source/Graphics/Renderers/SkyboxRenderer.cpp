#include "Graphics/Renderers/SkyboxRenderer.h"

#include "Application/Application.h"
#include "Application/GlobalSettings.h"
#include "Core/Memory.h"
#include "Graphics/MeshFactory.h"
#include "Graphics/Vulkan/Utilities.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {

    SkyboxRenderer::SkyboxRenderer(RenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) {
        std::vector<std::string> skyboxTextures1 = {
            "../Res/Textures/skybox/posx.jpg", "../Res/Textures/skybox/negx.jpg", "../Res/Textures/skybox/posy.jpg",
            "../Res/Textures/skybox/negy.jpg", "../Res/Textures/skybox/posz.jpg", "../Res/Textures/skybox/negz.jpg"};
        std::vector<std::string> skyboxTextures = {
            "../Res/Textures/stormy_skybox/stormydays_ft.tga", "../Res/Textures/stormy_skybox/stormydays_bk.tga",
            "../Res/Textures/stormy_skybox/stormydays_up.tga", "../Res/Textures/stormy_skybox/stormydays_dn.tga",
            "../Res/Textures/stormy_skybox/stormydays_rt.tga", "../Res/Textures/stormy_skybox/stormydays_lf.tga"};
        m_Material = std::make_shared<Material>(skyboxTextures, MaterialTexType::TextureCube);
        m_CubeMesh = std::make_shared<Mesh>(*createMesh(PrimativeShape::CUBE));

        m_SkyboxModel = new Entity(m_CubeMesh, m_Material);
        init(renderPass, windowWidth, windowHeight);
    }

    SkyboxRenderer::~SkyboxRenderer() {
        delete m_Pipeline;
        delete m_UniformBuffer;
        delete m_DescriptorSet;
        delete m_SkyboxModel;
    }

    void SkyboxRenderer::init(RenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) {
        m_Material->loadTextures();
        createGraphicsPipeline(renderPass, windowWidth, windowHeight);
        prepareUniformBuffer();
        createDescriptorSet();
    }

    void SkyboxRenderer::prepareScene() {
        resetCommandQueue();
        submit(m_SkyboxModel);
    }

    void SkyboxRenderer::present(CommandBuffer* commandBuffer) {
        if (GlobalSettings::instance()->displayBackground) {
            for (auto command : m_CommandQueue) {
                vkCmdBindDescriptorSets(commandBuffer->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        m_Pipeline->getPipelineLayout(), 0, 1, &m_DescriptorSet->getDescriptorSet(0), 0,
                                        nullptr);
                command.entity->getMesh()->getVertexBuffer()->bindVertex(commandBuffer, 0);
                command.entity->getMesh()->getIndexBuffer()->bindIndex(commandBuffer, VK_INDEX_TYPE_UINT32);
                m_Pipeline->setActive(*commandBuffer);

                auto indexCount = command.entity->getMesh()->getIndexBuffer()->getSize() / sizeof(uint32_t);
                vkCmdDrawIndexed(commandBuffer->getCommandBuffer(), static_cast<uint32_t>(indexCount), 1, 0, 0, 0);
                updateUniformBuffer(0);
            }
        }
    }

    void SkyboxRenderer::onResize(RenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) {
        // Cleanup
        {
            delete m_Pipeline;
            delete m_UniformBuffer;
        }
        createGraphicsPipeline(renderPass, newWidth, newHeight);
        prepareUniformBuffer();
        createDescriptorSet();
    }

    void SkyboxRenderer::createGraphicsPipeline(RenderPass* renderPass, uint32_t width, uint32_t height) {
        Shader       skyboxShader("../Res/Shaders/Skybox", "skybox.shader");
        PipelineInfo pipelineInfo = {};
        pipelineInfo.shader = &skyboxShader;

        pipelineInfo.renderpass = renderPass;
        pipelineInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
        pipelineInfo.depthTestEnable = VK_FALSE;
        pipelineInfo.depthWriteEnable = VK_FALSE;
        pipelineInfo.maxObjects = 2;

        // location, binding, format, offset
        VkVertexInputAttributeDescription pos = {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos)};
        pipelineInfo.vertexInputAttributes = {pos};

        // binding, descriptorType, descriptorCount, stageFlags, pImmuatbleSamplers
        VkDescriptorSetLayoutBinding viewProj = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT,
                                                 nullptr};
        VkDescriptorSetLayoutBinding sampler = {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,
                                                VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
        pipelineInfo.layoutBindings = {viewProj, sampler};
        pipelineInfo.width = width;
        pipelineInfo.height = height;
        pipelineInfo.pushConstants = {VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(int)};
        pipelineInfo.bindingDescription = {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX};

        m_Pipeline = new Pipeline();
        m_Pipeline->init(pipelineInfo);
    }

    void SkyboxRenderer::createDescriptorSet() {
        DescriptorSetInfo descriptorSetInfo;
        descriptorSetInfo.descriptorSetCount = 1;
        descriptorSetInfo.pipeline = m_Pipeline;

        m_DescriptorSet = new DescriptorSet();
        m_DescriptorSet->init(descriptorSetInfo);

        std::vector<BufferInfo> bufferInfos = {};
        BufferInfo              viewBufferInfo = {};
        viewBufferInfo.buffer = m_UniformBuffer->getBuffer();
        viewBufferInfo.offset = 0;
        viewBufferInfo.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        viewBufferInfo.size = sizeof(UniformVS);
        viewBufferInfo.binding = 0;
        viewBufferInfo.descriptorCount = 1;
        bufferInfos.push_back(viewBufferInfo);

        BufferInfo imageBufferInfo = {};
        imageBufferInfo.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        imageBufferInfo.binding = 1;
        imageBufferInfo.descriptorCount = 1;
        imageBufferInfo.imageSamplers.push_back(m_SkyboxModel->getMaterial()->getTextureImage()->getSampler());
        imageBufferInfo.imageViews.push_back(m_SkyboxModel->getMaterial()->getTextureImage()->getImageView());
        bufferInfos.push_back(imageBufferInfo);

        m_DescriptorSet->update(bufferInfos);
    }

    void SkyboxRenderer::prepareUniformBuffer() {
        VkDeviceSize viewBufferSize = sizeof(UniformVS);
        m_UniformBuffer = new Buffer(BufferUsage::UNIFORM, viewBufferSize, nullptr);
    }

    void SkyboxRenderer::updateUniformBuffer(uint32_t index) {
        UniformVS skyboxVS = {};

        skyboxVS.view = Application::getAppInstance()->getWindow()->getCamera()->getViewMatrix();
        skyboxVS.projection = Application::getAppInstance()->getWindow()->getCamera()->getProjectionMatrix();
        skyboxVS.projection[1][1] *= -1;

        skyboxVS.view[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        m_UniformBuffer->setData(sizeof(skyboxVS), &skyboxVS);
    }
}  // namespace Yare::Graphics
