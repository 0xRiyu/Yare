#ifndef YAREZO_SKYBOX_RENDERER_H
#define YAREZO_SKYBOX_RENDERER_H

#include "Graphics/Renderers/YzRenderer.h"

#include "Graphics/Vulkan/Vk_Pipeline.h"
#include "Graphics/Vulkan/Vk_Buffer.h"
#include "Graphics/Vulkan/Vk_DescriptorSet.h"

#include <memory>

namespace Yarezo::Graphics {

    class SkyboxRenderer : public YzRenderer {
    public:
        SkyboxRenderer();
        ~SkyboxRenderer() override;

        virtual void init(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) override;
        virtual void prepareScene() override;
        virtual void present(YzVkCommandBuffer* commandBuffer) override;
        virtual void onResize(YzVkRenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) override;

    private:
        void createGraphicsPipeline(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight);
        void createDescriptorSet();
        void prepareUniformBuffer();
        void updateUniformBuffer(uint32_t index);

    private:
        std::shared_ptr<Mesh> m_CubeMesh;
        MeshInstance* m_SkyboxModel;
        YzVkPipeline* m_Pipeline;
        YzVkDescriptorSet* m_DescriptorSet;
        YzVkBuffer* m_UniformBuffer;
    };
}

#endif // YAREZO_SKYBOX_RENDERER_H
