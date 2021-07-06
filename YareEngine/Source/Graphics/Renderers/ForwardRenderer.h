#ifndef YARE_FORWARD_RENDERER_H
#define YARE_FORWARD_RENDERER_H

#include "Graphics/Renderers/Renderer.h"
#include "Graphics/Vulkan/Pipeline.h"
#include "Graphics/Vulkan/Buffer.h"
#include "Graphics/Vulkan/DescriptorSet.h"

#include <memory>

#define MAX_OBJECTS 2048

namespace Yare::Graphics {

    class ForwardRenderer  : public Renderer {
    public:
        ForwardRenderer(RenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight);
        ~ForwardRenderer() override;

        void prepareScene() override;
        void present(CommandBuffer* commandBuffer) override;
        void onResize(RenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) override;

    private:
        void init(RenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) override;
        void createGraphicsPipeline(RenderPass* renderPass, uint32_t width, uint32_t height);
        void createDescriptorSets();
        void prepareUniformBuffers();
        void updateUniformBuffers(uint32_t index, const Transform& transform);

        // TODO Move this into some content management class
        std::vector<std::shared_ptr<Mesh>> m_Meshes;
        std::vector<std::shared_ptr<Material>> m_Materials;
        std::vector<std::shared_ptr<Entity>> m_Entities;

        uint64_t m_DynamicAlignment = 0;

        Pipeline*  m_Pipeline;
        DescriptorSet* m_DescriptorSet;

        struct UniformBuffers {
            Buffer* view;
            Buffer* dynamic;
        } m_UniformBuffers;

        UboDataDynamic m_UboDynamicData;
    };

}

#endif // YARE_FORWARD_RENDERER_H
