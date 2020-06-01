#ifndef YAREZO_IMGUI_RENDERER_H
#define YAREZO_IMGUI_RENDERER_H

#include "Graphics/Renderers/YzRenderer.h"

#include "Graphics/Vulkan/Vk.h"
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

#include <imgui/imgui.h>

namespace Yarezo::Graphics {

    class ImGuiRenderer : public YzRenderer {
    public:
        ImGuiRenderer();
        ~ImGuiRenderer();
        virtual void init(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) override;
        virtual void prepareScene() override;
        virtual void present(YzVkCommandBuffer* commandBuffer) override;
        virtual void onResize(YzVkRenderPass* renderPass, uint32_t newWidth, uint32_t newHeight) override;

    protected:
        void initResources(YzVkRenderPass* renderPass);
        void newFrame();
        void postFrame();
        void updateBuffers();

        struct PushConstBlock {
            glm::vec2 scale;
            glm::vec2 translate;
        };

    private:
        YzVkImage* m_Font;
        YzVkPipeline* m_Pipeline;
        YzVkBuffer* m_IndexBuffer = nullptr;
        YzVkBuffer* m_VertexBuffer = nullptr;
        YzVkDescriptorSet* m_DescriptorSet;
        PushConstBlock pushConstBlock = {};
    };

}

#endif //YAREZO_IMGUI_RENDERER_H
