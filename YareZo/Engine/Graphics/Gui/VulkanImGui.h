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

        // Options and values to display/toggle from the UI
        struct UISettings {
            bool displayModels = true;
            bool displayLogos = true;
            bool displayBackground = true;
            bool animateLight = false;
            float lightSpeed = 0.25f;
            std::array<float, 50> frameTimes{};
            float frameTimeMin = 9999.0f, frameTimeMax = 0.0f;
            float lightTimer = 0.0f;
        } uiSettings;

        struct PushConstBlock {
            glm::vec2 scale;
            glm::vec2 translate;
        };

        VulkanImGui();
        ~VulkanImGui();

        void init(size_t width, size_t height);
        void initResources(YzVkRenderPass* yzRenderPass);
        void newFrame();
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
