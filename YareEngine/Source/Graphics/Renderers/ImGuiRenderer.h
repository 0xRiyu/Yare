#ifndef YARE_IMGUI_RENDERER_H
#define YARE_IMGUI_RENDERER_H

#include "Graphics/Renderers/YzRenderer.h"

#include "Graphics/Vulkan/Pipeline.h"
#include "Graphics/Vulkan/Buffer.h"
#include "Graphics/Vulkan/DescriptorSet.h"

#include <imgui/imgui.h>

namespace Yare::Graphics {

    class ImGuiRenderer : public YzRenderer {
    public:
        ImGuiRenderer(RenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight);
        ~ImGuiRenderer();
        void prepareScene() override;
        void present(CommandBuffer* commandBuffer) override;
        void onResize(RenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) override;

    private:
        void init(RenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) override;
        void createGraphicsPipeline(RenderPass* renderPass);
        void createDescriptorSet();
        void newFrame();
        void postFrame();
        void updateBuffers();

        struct PushConstBlock {
            glm::vec2 scale = {};
            glm::vec2 translate = {};
        } m_PushConstBlock;

        Image* m_Font;
        Pipeline* m_Pipeline;
        Buffer* m_IndexBuffer = nullptr;
        Buffer* m_VertexBuffer = nullptr;
        DescriptorSet* m_DescriptorSet;
    };

}

#endif //YARE_IMGUI_RENDERER_H
