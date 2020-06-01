#ifndef YAREZO_FORWARD_RENDERER_H
#define YAREZO_FORWARD_RENDERER_H

#include "Graphics/Renderers/YzRenderer.h"

#include "Graphics/Vulkan/Vk_Pipeline.h"
#include "Graphics/Vulkan/Vk_Buffer.h"
#include "Graphics/Vulkan/Vk_DescriptorSet.h"

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

        void createGraphicsPipeline(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight);
        void createDescriptorSets();
        void prepareUniformBuffers();
        void updateUniformBuffers(uint32_t index, const glm::mat4& modelMatrix);

    private:
        // TODO Move this into some content management class, I have no clue what that would look like
        std::vector<Model*> m_Models;
        Material* m_DefaultMaterial;

        size_t m_DynamicAlignment = 0;

        YzVkPipeline*  m_Pipeline;
        YzVkDescriptorSet* m_DescriptorSet;

        struct UniformBuffers {
            YzVkBuffer* view;
            YzVkBuffer* dynamic;
        } m_UniformBuffers;

        UboDataDynamic m_UboDynamicData;
};

}

#endif // YAREZO_FORWARD_RENDERER_H
