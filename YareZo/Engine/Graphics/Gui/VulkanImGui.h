#ifndef YAREZO_VULKAN_IMGUI_H
#define YAREZO_VULKAN_IMGUI_H

#include <imgui/imgui.h>
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

namespace Yarezo::Graphics {

    class VulkanImGui {
    public:

        struct PushConstBlock {
            glm::vec2 scale;
            glm::vec2 translate;
        };

        VulkanImGui();
        ~VulkanImGui();

        void init(size_t width, size_t height);
        void initResources(YzVkRenderPass* yzRenderPass);
        void newFrame();
        void postFrame();
        void updateBuffers();
        void drawFrame(YzVkCommandBuffer* commandBuffer);
        void updateInputs();

    private:
        YzVkImage* m_Font;
        YzVkPipeline* m_Pipeline;
        YzVkBuffer* m_IndexBuffer = nullptr;
        YzVkBuffer* m_VertexBuffer = nullptr;
        YzVkDescriptorSet* m_DescriptorSet;
        PushConstBlock pushConstBlock = {};
    };

}


#endif //YAREZO_VULKAN_IMGUI_H
