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
        SkyboxRenderer(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight);
        ~SkyboxRenderer() override;

        void prepareScene() override;
        void present(YzVkCommandBuffer* commandBuffer) override;
        void onResize(YzVkRenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) override;

    private:
        void init(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) override;
        void createGraphicsPipeline(YzVkRenderPass* renderPass, uint32_t width, uint32_t height);
        void createDescriptorSet();
        void prepareUniformBuffer();
        void updateUniformBuffer(uint32_t index);

    private:
        std::shared_ptr<Mesh> m_CubeMesh;
        std::shared_ptr<Material> m_Material;
        MeshInstance* m_SkyboxModel;
        YzVkPipeline* m_Pipeline;
        YzVkDescriptorSet* m_DescriptorSet;
        YzVkBuffer* m_UniformBuffer;
    };
}

#endif // YAREZO_SKYBOX_RENDERER_H
