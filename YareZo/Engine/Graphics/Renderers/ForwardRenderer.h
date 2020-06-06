#ifndef YAREZO_FORWARD_RENDERER_H
#define YAREZO_FORWARD_RENDERER_H

#include "Graphics/Renderers/YzRenderer.h"

#include "Graphics/Vulkan/Vk_Pipeline.h"
#include "Graphics/Vulkan/Vk_Buffer.h"
#include "Graphics/Vulkan/Vk_DescriptorSet.h"

#include <memory>

#define MAX_OBJECTS 2048

namespace Yarezo::Graphics {


    class ForwardRenderer  : public YzRenderer {
    public:
        ForwardRenderer(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight);
        ~ForwardRenderer() override;

        void prepareScene() override;
        void present(YzVkCommandBuffer* commandBuffer) override;
        void onResize(YzVkRenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) override;

    private:
        void init(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) override;
        void createGraphicsPipeline(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight);
        void createDescriptorSets();
        void prepareUniformBuffers();
        void updateUniformBuffers(uint32_t index, const Transform& transform);

        // TODO Move this into some content management class
        std::vector<std::shared_ptr<Mesh>> m_Meshes;
        std::vector<std::shared_ptr<Material>> m_Materials;
        std::vector<std::shared_ptr<MeshInstance>> m_MeshInstances;

        uint64_t m_DynamicAlignment = 0;

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
