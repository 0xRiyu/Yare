#ifndef YARE_IMGUI_RENDERER_H
#define YARE_IMGUI_RENDERER_H

#include <imgui/imgui.h>

#include "Graphics/Renderers/Renderer.h"
#include "Graphics/Vulkan/Buffer.h"
#include "Graphics/Vulkan/DescriptorSet.h"
#include "Graphics/Vulkan/Pipeline.h"

namespace Yare::Graphics {

    class ImGuiRenderer : public Renderer {
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

        Image*         m_Font;
        Pipeline*      m_Pipeline;
        Buffer*        m_IndexBuffer = nullptr;
        Buffer*        m_VertexBuffer = nullptr;
        DescriptorSet* m_DescriptorSet;
    };

}  // namespace Yare::Graphics

#endif  // YARE_IMGUI_RENDERER_H
