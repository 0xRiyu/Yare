#include "Graphics/Renderers/ForwardRenderer.h"

#include "Graphics/Vulkan/Vk_Utilities.h"
#include "Graphics/Vulkan/Vk_Devices.h"
#include "Application/GlobalSettings.h"

#include "Core/Yzh.h"
#include "Core/Memory.h"

namespace Yarezo::Graphics {

    ForwardRenderer::ForwardRenderer() {
        m_Meshes.emplace_back(std::make_shared<Mesh>("../YareZo/Resources/Models/viking_room.obj"));
        m_Meshes.emplace_back(std::make_shared<Mesh>("../YareZo/Resources/Models/cube.obj"));
        //m_Models.emplace_back(new Model("../YareZo/Resources/Models/chalet.obj", "../YareZo/Resources/Textures/chalet.jpg"));
        m_MeshInstances.emplace_back(new MeshInstance(m_Meshes[0].get(),  "../YareZo/Resources/Textures/viking_room.png"));
        m_MeshInstances.emplace_back(new MeshInstance(m_Meshes[1].get(), "../YareZo/Resources/Textures/crate.png"));

        Transform transform{glm::vec3(0.0f, -0.15f, -1.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(1.0f, 1.0f, 1.0f)};
        m_MeshInstances[0]->setTransform(transform);

        Transform transform2{glm::vec3(0.0f, 0.0f, 1.0f),
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(1.0f, 1.0f, 1.0f)};
        m_MeshInstances[1]->setTransform(transform2);
    }

    ForwardRenderer::~ForwardRenderer() {
        if (m_UboDynamicData.model) {
            alignedFree(m_UboDynamicData.model);
        }

        delete m_Pipeline;

        delete m_UniformBuffers.view;
        delete m_UniformBuffers.dynamic;

        for (auto model : m_MeshInstances){
            delete model;
        }

        delete m_DefaultMaterial;
    }

    void ForwardRenderer::init(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) {
        for (auto mesh: m_MeshInstances) {
            mesh->load(MaterialTexType::Texture2D);
        }
        m_DefaultMaterial = new Material();

        createGraphicsPipeline(renderPass, windowWidth, windowHeight);

        prepareUniformBuffers();

        createDescriptorSets();
    }


    void ForwardRenderer::prepareScene() {
        resetCommandQueue();

        submit(m_MeshInstances[0]);

        submit(m_MeshInstances[1]);
    }

    void ForwardRenderer::present(YzVkCommandBuffer* commandBuffer) {
        int index = 0;

        if (GlobalSettings::instance()->displayModels) {
            for (auto& command : m_CommandQueue) {

                updateUniformBuffers(index, command.meshInstance->getTransform());
                uint32_t dynamicOffset = index * static_cast<uint32_t>(m_DynamicAlignment);
                m_Pipeline->setActive(*commandBuffer);
                int imageIdx = command.meshInstance->getImageIdx();

                vkCmdPushConstants(commandBuffer->getCommandBuffer(), m_Pipeline->getPipelineLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(int), (void *)&imageIdx);
                vkCmdBindDescriptorSets(commandBuffer->getCommandBuffer(),
                                        VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->getPipelineLayout(), 0u,
                                        1u, &m_DescriptorSet->getDescriptorSet(0), 1, &dynamicOffset);

                command.meshInstance->getMesh()->getVertexBuffer()->bindVertex(commandBuffer, 0);
                command.meshInstance->getMesh()->getIndexBuffer()->bindIndex(commandBuffer, VK_INDEX_TYPE_UINT32);

                vkCmdDrawIndexed(commandBuffer->getCommandBuffer(), static_cast<uint32_t>(command.meshInstance->getMesh()->getIndexBuffer()->getSize() / sizeof(uint32_t)), 1, 0, 0, 0);
                index++;
            }
        }
    }

    void ForwardRenderer::onResize(YzVkRenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) {
        // Cleanup
        {
            if (m_UboDynamicData.model) {
                alignedFree(m_UboDynamicData.model);
            }

            delete m_Pipeline;

            delete m_UniformBuffers.view;
            delete m_UniformBuffers.dynamic;

        }
        createGraphicsPipeline(renderPass, newWidth, newHeight);
        prepareUniformBuffers();
        createDescriptorSets();
    }

    void ForwardRenderer::createGraphicsPipeline(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) {
        YzVkShader shader("../YareZo/Resources/Shaders", "texture_array.shader");

        PipelineInfo pipelineInfo = {};
        pipelineInfo.shader = &shader;
        pipelineInfo.renderpass = renderPass;
        pipelineInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        pipelineInfo.depthTestEnable = VK_TRUE;
        pipelineInfo.depthWriteEnable = VK_TRUE;
        pipelineInfo.maxObjects = 2;
        pipelineInfo.width = windowWidth;
        pipelineInfo.height = windowHeight;
        pipelineInfo.pushConstants = {VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(int)};
        pipelineInfo.bindingDescription =  VkVertexInputBindingDescription{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX};

        pipelineInfo.vertexInputAttributes.emplace_back(VkVertexInputAttributeDescription{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos)});
        pipelineInfo.vertexInputAttributes.emplace_back(VkVertexInputAttributeDescription{1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
        pipelineInfo.vertexInputAttributes.emplace_back(VkVertexInputAttributeDescription{2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord)});


        //                                       Binding, DescriptorType, DescriptorCount, StageFlags, pImmuatbleSamplers
        pipelineInfo.layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                                                              1, VK_SHADER_STAGE_VERTEX_BIT, nullptr});
        pipelineInfo.layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                                                                              1, VK_SHADER_STAGE_VERTEX_BIT, nullptr});
        pipelineInfo.layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                              MAX_NUM_TEXTURES, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr});

        m_Pipeline = new YzVkPipeline();
        m_Pipeline->init(pipelineInfo);
    }

    void ForwardRenderer::createDescriptorSets() {

        DescriptorSetInfo descriptorSetInfo;
        descriptorSetInfo.descriptorSetCount = 1;
        descriptorSetInfo.pipeline = m_Pipeline;

        // First create the descriptor set, but the buffers are empty
        m_DescriptorSet = new YzVkDescriptorSet();
        m_DescriptorSet->init(descriptorSetInfo);


        std::vector<BufferInfo> bufferInfos = {};
        BufferInfo viewBufferInfo = {};
        viewBufferInfo.buffer = m_UniformBuffers.view->getBuffer();
        viewBufferInfo.offset = 0;
        viewBufferInfo.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        viewBufferInfo.size = sizeof(UniformVS);
        viewBufferInfo.binding = 0;
        viewBufferInfo.imageSampler = nullptr;
        viewBufferInfo.imageView = nullptr;
        viewBufferInfo.descriptorCount = 1;

        BufferInfo dynamicBufferInfo = {};
        dynamicBufferInfo.buffer = m_UniformBuffers.dynamic->getBuffer();
        dynamicBufferInfo.offset = 0;
        dynamicBufferInfo.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        dynamicBufferInfo.size = sizeof(glm::mat4);
        dynamicBufferInfo.binding = 1;
        dynamicBufferInfo.imageSampler = nullptr;
        dynamicBufferInfo.imageView = nullptr;
        dynamicBufferInfo.descriptorCount = 1;

        bufferInfos.push_back(viewBufferInfo);
        bufferInfos.push_back(dynamicBufferInfo);

        BufferInfo imageBufferInfo = {};
        imageBufferInfo.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        imageBufferInfo.binding = 2;
        imageBufferInfo.descriptorCount = MAX_NUM_TEXTURES;

        int imageIdx = 0;
        for (auto meshInstance : m_MeshInstances) {
            imageBufferInfo.imageSampler = meshInstance->getMaterial()->getTextureImage()->getSampler();
            imageBufferInfo.imageView = meshInstance->getMaterial()->getTextureImage()->getImageView();
            bufferInfos.push_back(imageBufferInfo);
            meshInstance->setImageIdx(imageIdx++);
        }

        for (size_t i = m_MeshInstances.size(); i < MAX_NUM_TEXTURES; i++) {

            imageBufferInfo.imageSampler = m_DefaultMaterial->getTextureImage()->getSampler();
            imageBufferInfo.imageView = m_DefaultMaterial->getTextureImage()->getImageView();
            bufferInfos.push_back(imageBufferInfo);
        }

        m_DescriptorSet->update(bufferInfos);
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

        VkDeviceSize dynamicBufferSize = MAX_OBJECTS * m_DynamicAlignment;
        m_UboDynamicData.model = (glm::mat4*)alignedAlloc(dynamicBufferSize, m_DynamicAlignment);

        VkMemoryPropertyFlags dynamicPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

        m_UniformBuffers.view = new YzVkBuffer(usageFlags, viewPropertyFlags, viewBufferSize, nullptr);
        m_UniformBuffers.dynamic = new YzVkBuffer(usageFlags, dynamicPropertyFlags, dynamicBufferSize, nullptr);
    }

    void ForwardRenderer::updateUniformBuffers(uint32_t index, const Transform& transform) {
        // TODO, store UBOs for each model we want to display in one UBO, separated by an offset
        // then bind based on that offset in the present call
        glm::mat4* uboDynamicModelPtr = (glm::mat4*)((uint64_t)m_UboDynamicData.model + (index * m_DynamicAlignment));
        *uboDynamicModelPtr = transform.getMatrix();

        m_UniformBuffers.dynamic->setDynamicData(MAX_OBJECTS * m_DynamicAlignment, &*m_UboDynamicData.model);

        UniformVS uboVS = {};
        uboVS.view = Application::getAppInstance()->getWindow()->getCamera()->getViewMatrix();
        uboVS.projection = Application::getAppInstance()->getWindow()->getCamera()->getProjectionMatrix();
        uboVS.projection[1][1] *= -1;

        m_UniformBuffers.view->setData(sizeof(uboVS), &uboVS);
    }
}
