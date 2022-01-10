#include "Graphics/Renderers/ForwardRenderer.h"

#include "Application/Application.h"
#include "Application/GlobalSettings.h"
#include "Core/Memory.h"
#include "Graphics/MeshFactory.h"
#include "Graphics/Vulkan/Devices.h"
#include "Graphics/Vulkan/Utilities.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {

    ForwardRenderer::ForwardRenderer(RenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) {
        m_Meshes.push_back(std::make_shared<Mesh>("../Res/Models/viking_room.obj"));
        m_Meshes.emplace_back(createMesh(PrimativeShape::CUBE));
        m_Meshes.emplace_back(createQuadPlane(15, 15));
        m_Meshes.push_back(std::make_shared<Mesh>("../Res/Models/Lowpoly_tree_sample.obj"));

        m_Materials.push_back(std::make_shared<Material>());  // Default texture
        m_Materials.push_back(std::make_shared<Material>("../Res/Textures/viking_room.png"));
        m_Materials.push_back(std::make_shared<Material>("../Res/Textures/mossytiles.jpg"));
        m_Materials.push_back(std::make_shared<Material>("../Res/Textures/skysphere.png"));
        m_Materials.push_back(std::make_shared<Material>("../Res/Textures/sprite.jpg"));
        m_Materials.push_back(std::make_shared<Material>("../Res/Textures/tile.png"));

        Transform transform{glm::vec3(3.0f, -0.42f, 0.0f), glm::radians(glm::vec3(90.0f, 90.0f, -180.0f)),
                            glm::vec3(1.0f, 1.0f, 1.0f)};
        m_Entities.push_back(std::make_shared<Entity>(m_Meshes[0], m_Materials[1], transform));

        Transform transform2;
        Transform transform3;
        transform3.setScale(glm::vec3(0.05, 0.05, 0.05));
        transform3.setTranslation(1.5f, -0.5f, 0.0f);
        m_Entities.push_back(std::make_shared<Entity>(m_Meshes[3], m_Materials[0], transform3));
        transform2.setTranslation(-7.5f, -0.5f, -7.5f);
        m_Entities.push_back(std::make_shared<Entity>(m_Meshes[2], m_Materials[2], transform2));
        transform2.setTranslation(0.0f, 0.0f, 0.0f);
        m_Entities.push_back(std::make_shared<Entity>(m_Meshes[1], m_Materials[4], transform2));
        transform2.setTranslation(-1.5f, 0.0f, 0.0f);
        m_Entities.push_back(std::make_shared<Entity>(m_Meshes[1], m_Materials[3], transform2));

        init(renderPass, windowWidth, windowHeight);
    }

    ForwardRenderer::~ForwardRenderer() {
        if (m_UboDynamicData.model) {
            alignedFree(m_UboDynamicData.model);
        }

        delete m_Pipeline;

        delete m_UniformBuffers.view;
        delete m_UniformBuffers.dynamic;
    }

    void ForwardRenderer::init(RenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) {
        for (auto material : m_Materials) {
            material->loadTextures();
        }

        createGraphicsPipeline(renderPass, windowWidth, windowHeight);

        prepareUniformBuffers();

        createDescriptorSets();
    }

    void ForwardRenderer::prepareScene() {
        resetCommandQueue();

        for (const auto entity : m_Entities) {
            submit(entity.get());
        }
    }

    void ForwardRenderer::present(CommandBuffer* commandBuffer) {
        if (GlobalSettings::instance()->displayModels) {
            int index = 0;
            for (auto& command : m_CommandQueue) {
                uint32_t dynamicOffset = index * static_cast<uint32_t>(m_DynamicAlignment);
                updateUniformBuffers(index, command.entity->getTransform());
                m_Pipeline->setActive(*commandBuffer);

                int imageIdx = command.entity->getMaterial()->getImageIdx();
                vkCmdPushConstants(commandBuffer->getCommandBuffer(), m_Pipeline->getPipelineLayout(),
                                   VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(int), (void*)&imageIdx);

                vkCmdBindDescriptorSets(commandBuffer->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        m_Pipeline->getPipelineLayout(), 0u, 1u, &m_DescriptorSet->getDescriptorSet(0),
                                        1, &dynamicOffset);

                command.entity->getMesh()->getVertexBuffer()->bindVertex(commandBuffer, 0);
                command.entity->getMesh()->getIndexBuffer()->bindIndex(commandBuffer, VK_INDEX_TYPE_UINT32);

                auto indicesCount = command.entity->getMesh()->getIndexBuffer()->getSize() / sizeof(uint32_t);
                vkCmdDrawIndexed(commandBuffer->getCommandBuffer(), static_cast<uint32_t>(indicesCount), 1, 0, 0, 0);
                index++;
            }
        }
    }

    void ForwardRenderer::onResize(RenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) {
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

    void ForwardRenderer::createGraphicsPipeline(RenderPass* renderPass, uint32_t width, uint32_t height) {
        Shader shader("../Res/Shaders", "texture_array.shader");

        PipelineInfo pInfo = {};
        pInfo.shader = &shader;
        pInfo.renderpass = renderPass;
        pInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        pInfo.depthTestEnable = VK_TRUE;
        pInfo.depthWriteEnable = VK_TRUE;
        pInfo.maxObjects = 2;
        pInfo.width = width;
        pInfo.height = height;
        pInfo.pushConstants = {VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(int)};
        pInfo.bindingDescription = VkVertexInputBindingDescription{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX};

        // location, binding, format, offset
        VkVertexInputAttributeDescription pos = {0u, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos)};
        VkVertexInputAttributeDescription uv = {2u, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)};
        VkVertexInputAttributeDescription normal = {1u, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)};
        pInfo.vertexInputAttributes = {pos, uv, normal};

        // binding, descriptorType, descriptorCount, stageFlags, pImmuatbleSamplers
        VkDescriptorSetLayoutBinding projView = {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT,
                                                 nullptr};
        VkDescriptorSetLayoutBinding model = {1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1,
                                              VK_SHADER_STAGE_VERTEX_BIT, nullptr};
        VkDescriptorSetLayoutBinding sampler = {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, (std::min)(256u, Devices::instance()->getGPUProperties().limits.maxPerStageDescriptorSamplers),
                                                VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
        pInfo.layoutBindings = {projView, model, sampler};

        m_Pipeline = new Pipeline();
        m_Pipeline->init(pInfo);
    }

    void ForwardRenderer::createDescriptorSets() {
        DescriptorSetInfo descriptorSetInfo;
        descriptorSetInfo.descriptorSetCount = 1;
        descriptorSetInfo.pipeline = m_Pipeline;

        // First create the descriptor set, but the buffers are empty
        m_DescriptorSet = new DescriptorSet();
        m_DescriptorSet->init(descriptorSetInfo);

        std::vector<BufferInfo> bufferInfos = {};
        BufferInfo              viewBufferInfo = {};
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
        imageBufferInfo.descriptorCount = (std::min)(256u, Devices::instance()->getGPUProperties().limits.maxPerStageDescriptorSamplers);

        int imageIdx = 0;
        for (auto material : m_Materials) {
            imageBufferInfo.imageSampler = material->getTextureImage()->getSampler();
            imageBufferInfo.imageView = material->getTextureImage()->getImageView();
            bufferInfos.push_back(imageBufferInfo);
            material->setImageIdx(imageIdx++);
        }

        for (size_t i = m_Materials.size(); i < (std::min)(256u, Devices::instance()->getGPUProperties().limits.maxPerStageDescriptorSamplers); i++) {
            imageBufferInfo.imageSampler = m_Materials[0]->getTextureImage()->getSampler();
            imageBufferInfo.imageView = m_Materials[0]->getTextureImage()->getImageView();
            bufferInfos.push_back(imageBufferInfo);
        }

        m_DescriptorSet->update(bufferInfos);
    }

    void ForwardRenderer::prepareUniformBuffers() {
        VkDeviceSize viewBufferSize = sizeof(UniformVS);

        m_DynamicAlignment = sizeof(glm::mat4);
        auto minUboAlignment = Devices::instance()->getGPUProperties().limits.minUniformBufferOffsetAlignment;
        if (minUboAlignment > 0) {
            m_DynamicAlignment = (m_DynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
        }

        VkDeviceSize dynamicBufferSize = MAX_OBJECTS * m_DynamicAlignment;
        m_UboDynamicData.model = (glm::mat4*)alignedAlloc(dynamicBufferSize, m_DynamicAlignment);

        m_UniformBuffers.view = new Buffer(BufferUsage::UNIFORM, viewBufferSize, nullptr);
        m_UniformBuffers.dynamic = new Buffer(BufferUsage::DYNAMIC, dynamicBufferSize, nullptr);
    }

    void ForwardRenderer::updateUniformBuffers(uint32_t index, const Transform& transform) {
        // TODO, store UBOs for each model we want to display in one UBO, separated by an offset
        // then bind based on that offset in the present call
        glm::mat4* uboDynamicModelPtr = (glm::mat4*)((uint64_t)m_UboDynamicData.model + (index * m_DynamicAlignment));
        *uboDynamicModelPtr = transform.getMatrix();

        m_UniformBuffers.dynamic->setDynamicData(sizeof(glm::mat4), uboDynamicModelPtr, index * m_DynamicAlignment);

        UniformVS uboVS = {};
        uboVS.view = Application::getAppInstance()->getWindow()->getCamera()->getViewMatrix();
        uboVS.projection = Application::getAppInstance()->getWindow()->getCamera()->getProjectionMatrix();
        uboVS.projection[1][1] *= -1;

        m_UniformBuffers.view->setData(sizeof(uboVS), &uboVS);
    }
}  // namespace Yare::Graphics
