#ifndef YARE_SKYBOX_RENDERER_H
#define YARE_SKYBOX_RENDERER_H

#include "Graphics/Renderers/YzRenderer.h"

#include "Graphics/Vulkan/Pipeline.h"
#include "Graphics/Vulkan/Buffer.h"
#include "Graphics/Vulkan/DescriptorSet.h"

#include <memory>

namespace Yare::Graphics {

    class SkyboxRenderer : public YzRenderer {
    public:
        SkyboxRenderer(RenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight);
        ~SkyboxRenderer() override;

        void prepareScene() override;
        void present(CommandBuffer* commandBuffer) override;
        void onResize(RenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) override;

    private:
        void init(RenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) override;
        void createGraphicsPipeline(RenderPass* renderPass, uint32_t width, uint32_t height);
        void createDescriptorSet();
        void prepareUniformBuffer();
        void updateUniformBuffer(uint32_t index);

    private:
        std::shared_ptr<Mesh> m_CubeMesh;
        std::shared_ptr<Material> m_Material;
        MeshInstance* m_SkyboxModel;
        Pipeline* m_Pipeline;
        DescriptorSet* m_DescriptorSet;
        Buffer* m_UniformBuffer;
    };
}

#endif // YARE_SKYBOX_RENDERER_H
