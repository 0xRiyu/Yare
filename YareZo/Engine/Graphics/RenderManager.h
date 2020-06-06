#ifndef YAREZO_RENDER_MANAGER_H
#define YAREZO_RENDER_MANAGER_H

#include "Graphics/Vulkan/Vk_Renderer.h"
#include "Graphics/Vulkan/Vk_Framebuffer.h"
#include "Graphics/Vulkan/Vk_CommandBuffer.h"
#include "Graphics/Vulkan/Vk_RenderPass.h"
#include "Graphics/Vulkan/Vk_Image.h"

#include "Graphics/Window/YzWindow.h"

#include "Graphics/Renderers/YzRenderer.h"

namespace Yarezo::Graphics {

    class RenderManager {
    public:
        RenderManager(const std::shared_ptr<YzWindow> window);
        ~RenderManager();

        void renderScene();
        void begin();
        void end();
        void waitDeviceIdle();

    protected:
        void init();
        void createRenderPass();
        void createFrameBuffers();
        void createCommandBuffers();
        void onResize();


    private:
        // Constructs the instance, devices and swapchain required for rendering
        YzVkRenderer*                            m_VulkanRenderer;
        std::vector<YzVkFramebuffer*>            m_FrameBuffers;
        std::vector<YzVkCommandBuffer*>          m_CommandBuffers;
        YzVkRenderPass*                          m_RenderPass;
        YzVkImage*                               m_DepthBuffer;
        const std::shared_ptr<YzWindow>          m_WindowRef;
        // TODO: Find a better naming scheme
        std::vector<YzRenderer*>                 m_Renderers;

        uint32_t m_CurrentBufferID = 0;
        uint32_t m_WindowWidth = 0;
        uint32_t m_WindowHeight = 0;

    };
}

#endif //YAREZO_RENDER_MANAGER_H
