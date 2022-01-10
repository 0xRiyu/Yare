#ifndef YARE_RENDER_MANAGER_H
#define YARE_RENDER_MANAGER_H

#include "Graphics/Renderers/Renderer.h"
#include "Graphics/Vulkan/CommandBuffer.h"
#include "Graphics/Vulkan/Context.h"
#include "Graphics/Vulkan/Framebuffer.h"
#include "Graphics/Vulkan/Image.h"
#include "Graphics/Vulkan/Renderpass.h"
#include "Graphics/Window/Window.h"

namespace Yare::Graphics {

    class RenderManager {
       public:
        RenderManager(const std::shared_ptr<Window> window);
        ~RenderManager();

        void renderScene();
        void begin();
        void end();

       protected:
        void init();
        void createRenderPass();
        void createFrameBuffers();
        void createCommandBuffers();
        void onResize();

       private:
        // Constructs the instance, devices and swapchain required for rendering
        VulkanContext*                m_VulkanContext;
        std::vector<Framebuffer*>     m_FrameBuffers;
        std::vector<CommandBuffer*>   m_CommandBuffers;
        RenderPass*                   m_RenderPass;
        Image*                        m_DepthBuffer;
        const std::shared_ptr<Window> m_WindowRef;
        // TODO: Find a better naming scheme
        std::vector<Renderer*> m_Renderers;

        uint32_t m_CurrentBufferID = 0;
        uint32_t m_WindowWidth = 0;
        uint32_t m_WindowHeight = 0;
    };
}  // namespace Yare::Graphics

#endif  // YARE_RENDER_MANAGER_H
