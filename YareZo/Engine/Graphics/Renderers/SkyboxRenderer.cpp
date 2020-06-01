#include "Graphics/Renderers/SkyboxRenderer.h"
#include "Graphics/Vulkan/Vk_Utilities.h"
#include "Application/GlobalSettings.h"

#include "Core/Yzh.h"
#include "Core/Memory.h"

namespace Yarezo::Graphics {

    SkyboxRenderer::SkyboxRenderer() {
        std::vector<std::string> skyboxTextures = {
                                                   "../YareZo/Resources/Textures/skybox/posx.jpg",
                                                   "../YareZo/Resources/Textures/skybox/negx.jpg",
                                                   "../YareZo/Resources/Textures/skybox/posy.jpg",
                                                   "../YareZo/Resources/Textures/skybox/negy.jpg",
                                                   "../YareZo/Resources/Textures/skybox/posz.jpg",
                                                   "../YareZo/Resources/Textures/skybox/negz.jpg",
        };

        m_SkyboxModel = new Model("../YareZo/Resources/Models/cube.obj", skyboxTextures);
    }

    SkyboxRenderer::~SkyboxRenderer() {
        delete m_Pipeline;
        delete m_UniformBuffer;
        delete m_DescriptorSet;
        delete m_SkyboxModel;
    }

    void SkyboxRenderer::init(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) {
        m_SkyboxModel->load(MaterialTexType::TextureCube);
        createGraphicsPipeline(renderPass, windowWidth, windowHeight);
        prepareUniformBuffer();
        createDescriptorSet();
    }

    void SkyboxRenderer::prepareScene() {
        resetCommandQueue();
        submitModel(m_SkyboxModel, glm::mat4(1.0f) /* unused in a skybox */);
    }


    void SkyboxRenderer::present(YzVkCommandBuffer* commandBuffer) {
        if (GlobalSettings::instance()->displayBackground) {
            for (auto command : m_CommandQueue) {
                vkCmdBindDescriptorSets(commandBuffer->getCommandBuffer(),
                                        VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->getPipelineLayout(),
                                        0, 1, &m_DescriptorSet->getDescriptorSet(0), 0 , nullptr);
                command.model->getMesh()->getVertexBuffer()->bindVertex(*commandBuffer, 0);
                command.model->getMesh()->getIndexBuffer()->bindIndex(*commandBuffer, VK_INDEX_TYPE_UINT32);
                m_Pipeline->setActive(*commandBuffer);
                vkCmdDrawIndexed(commandBuffer->getCommandBuffer(),
                                 static_cast<uint32_t>(m_SkyboxModel->getMesh()->getIndexBuffer()->getSize() / sizeof(uint32_t)),
                                 1, 0, 0, 0);
                updateUniformBuffer(0, command.transform);
            }
        }
    }

    void SkyboxRenderer::onResize(YzVkRenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) {
        // Cleanup
        {
            delete m_Pipeline;
            delete m_UniformBuffer;
        }
        createGraphicsPipeline(renderPass, newWidth, newHeight);
        prepareUniformBuffer();
        createDescriptorSet();
    }

    void SkyboxRenderer::createGraphicsPipeline(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) {
        YzVkShader skyboxShader("../YareZo/Resources/Shaders", "skybox.shader");
        PipelineInfo pipelineInfo = {};
        pipelineInfo.shader = &skyboxShader;

        pipelineInfo.renderpass = renderPass;
        pipelineInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
        pipelineInfo.depthTestEnable = VK_FALSE;
        pipelineInfo.depthWriteEnable = VK_FALSE;
        pipelineInfo.maxObjects = 2;

        pipelineInfo.vertexInputAttributes.clear();
        pipelineInfo.vertexInputAttributes.emplace_back(VkVertexInputAttributeDescription{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos)});
        pipelineInfo.layoutBindings.clear();
        pipelineInfo.layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                                                              1, VK_SHADER_STAGE_VERTEX_BIT, nullptr});
        pipelineInfo.layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                              1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr});
        pipelineInfo.width = windowWidth;
        pipelineInfo.height = windowHeight;
        pipelineInfo.pushConstants = {VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(int)};
        pipelineInfo.bindingDescription =  VkVertexInputBindingDescription{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX};

        m_Pipeline = new YzVkPipeline();
        m_Pipeline->init(pipelineInfo);

    }

    void SkyboxRenderer::createDescriptorSet(){
        DescriptorSetInfo descriptorSetInfo;
        descriptorSetInfo.descriptorSetCount = 1;
        descriptorSetInfo.pipeline = m_Pipeline;

        m_DescriptorSet = new YzVkDescriptorSet();
        m_DescriptorSet->init(descriptorSetInfo);


        std::vector<BufferInfo> bufferInfos = {};
        BufferInfo viewBufferInfo = {};
        viewBufferInfo.buffer = m_UniformBuffer->getBuffer();
        viewBufferInfo.offset = 0;
        viewBufferInfo.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        viewBufferInfo.size = sizeof(UniformVS);
        viewBufferInfo.binding = 0;
        viewBufferInfo.imageSampler = nullptr;
        viewBufferInfo.imageView = nullptr;
        viewBufferInfo.descriptorCount = 1;
        bufferInfos.push_back(viewBufferInfo);

        BufferInfo imageBufferInfo = {};
        imageBufferInfo.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        imageBufferInfo.binding = 1;
        imageBufferInfo.descriptorCount = 1;
        imageBufferInfo.imageSampler = m_SkyboxModel->getMaterial()->getTextureImage()->getSampler();
        imageBufferInfo.imageView = m_SkyboxModel->getMaterial()->getTextureImage()->getImageView();
        bufferInfos.push_back(imageBufferInfo);

        m_DescriptorSet->update(bufferInfos);
    }

    void SkyboxRenderer::prepareUniformBuffer() {
        VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        VkMemoryPropertyFlags viewPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        VkDeviceSize viewBufferSize = sizeof(UniformVS);

        m_UniformBuffer = new YzVkBuffer(usageFlags, viewPropertyFlags, viewBufferSize, nullptr);
    }

    void SkyboxRenderer::updateUniformBuffer(uint32_t index, const glm::mat4& modelMatrix) {
        UniformVS skyboxVS = {};

        skyboxVS.view = Application::getAppInstance()->getWindow()->getCamera()->getViewMatrix();
        skyboxVS.projection = Application::getAppInstance()->getWindow()->getCamera()->getProjectionMatrix();
        skyboxVS.projection[1][1] *= -1;

        skyboxVS.view[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        m_UniformBuffer->setData(sizeof(skyboxVS), &skyboxVS);
    }
}
