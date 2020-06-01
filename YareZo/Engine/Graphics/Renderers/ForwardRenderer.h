#ifndef YAREZO_FORWARD_RENDERER_H
#define YAREZO_FORWARD_RENDERER_H

#include "Graphics/Renderers/YzRenderer.h"

#include "Core/DataStructures.h"

#include "Graphics/Vulkan/Vk_Instance.h"
#include "Graphics/Vulkan/Vk_Devices.h"
#include "Graphics/Vulkan/Vk_Swapchain.h"
#include "Graphics/Vulkan/Vk_Renderpass.h"
#include "Graphics/Vulkan/Vk_Pipeline.h"
#include "Graphics/Vulkan/Vk_Framebuffer.h"
#include "Graphics/Vulkan/Vk_CommandPool.h"
#include "Graphics/Vulkan/Vk_Buffer.h"
#include "Graphics/Vulkan/Vk_DescriptorSet.h"
#include "Graphics/Vulkan/Vk_CommandBuffer.h"
#include "Graphics/Vulkan/Vk_Semaphore.h"
#include "Graphics/Vulkan/Vk_Image.h"
#include "Graphics/Vulkan/Vk_Renderer.h"

#include "Graphics/Gui/VulkanImGui.h"

#define MAX_OBJECTS 2048

namespace Yarezo::Graphics {


    class ForwardRenderer  : public YzRenderer {
    public:
        ForwardRenderer();
        ~ForwardRenderer() override;

        virtual void init(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) override;
        virtual void prepareScene() override;
        virtual void present(YzVkCommandBuffer* commandBuffer) override;
        virtual void onResize(YzVkRenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) override;

        void submitModel(Model* model, const glm::mat4& transform);

        void createGraphicsPipeline(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight);
        void createDescriptorSets();
        void prepareUniformBuffers();
        void updateUniformBuffers(uint32_t index, const glm::mat4& modelMatrix);

    private:
        // TODO Move this into some content management class, I have no clue what that would look like
        std::vector<Model*> m_Models;
        Model* m_SkyboxModel;
        Material* m_DefaultMaterial;

        size_t m_DynamicAlignment = 0;

        struct {
            YzVkPipeline*  pipeline;
            YzVkPipeline*  skybox;
        } m_Pipelines;

        struct {
            YzVkDescriptorSet* descriptorSet;
            YzVkDescriptorSet* skybox;
        } m_DescriptorSets;

        struct UniformBuffers {
            YzVkBuffer* view;
            YzVkBuffer* dynamic;
            YzVkBuffer* skybox;
        } m_UniformBuffers;

        UboDataDynamic m_UboDynamicData;
};

}

#endif // YAREZO_FORWARD_RENDERER_H
