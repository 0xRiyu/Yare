#include "Graphics/RenderManager.h"

#include "Graphics/Vulkan/Vk_Utilities.h"
#include "Graphics/Renderers/ForwardRenderer.h"
#include "Graphics/Renderers/ImGuiRenderer.h"
#include "Graphics/Renderers/SkyboxRenderer.h"

namespace Yarezo::Graphics {

    RenderManager::RenderManager() {
        init();
    }

    RenderManager::~RenderManager() {
        for (auto renderer : m_Renderers){
            delete renderer;
        }

        delete m_DepthBuffer;

        for (auto commandBuffer : m_CommandBuffers) {
            delete commandBuffer;
        }
        m_CommandBuffers.clear();

        for (auto frameBuffer : m_FrameBuffers) {
            delete frameBuffer;
        }
        m_FrameBuffers.clear();

        delete m_RenderPass;

        delete m_Renderer;
    }

    void RenderManager::renderScene() {
        begin();
        for (const auto renderer : m_Renderers) {
            renderer->prepareScene();
            renderer->present(m_CommandBuffers[m_CurrentBufferID]);
        }
        end();
    }

    void RenderManager::begin() {

        // Renderer will ask the swapchain to get the next image (frame)
        // for us to work with, if the result is OUT_OF_DATA_KHR or SUBOPTIMAL_KHR
        // we need to re-create our pipeline
        if (!m_Renderer->begin()) {
            onResize();
        }

        m_CurrentBufferID = m_Renderer->getYzSwapchain()->getCurrentImage();

        m_CommandBuffers[m_CurrentBufferID]->beginRecording();

        m_RenderPass->beginRenderPass(m_CommandBuffers[m_CurrentBufferID], m_FrameBuffers[m_CurrentBufferID], m_Renderer->getYzSwapchain().get());
    }

    void RenderManager::end() {
        m_RenderPass->endRenderPass(m_CommandBuffers[m_CurrentBufferID]);

        m_CommandBuffers[m_CurrentBufferID]->endRecording();

        if (!m_Renderer->present(m_CommandBuffers[m_CurrentBufferID])) {
            onResize();
        }
    }

    void RenderManager::waitDeviceIdle() {
        YzVkDevice::instance()->waitIdle();
    }

    void RenderManager::init() {
        m_Renderer = new YzVkRenderer();
        m_WindowWidth =  m_Renderer->getYzSwapchain()->getExtent().width;
        m_WindowHeight = m_Renderer->getYzSwapchain()->getExtent().height;
        createRenderPass();
        createFrameBuffers();
        createCommandBuffers();
        m_Renderers.emplace_back(new SkyboxRenderer(m_RenderPass, m_WindowWidth, m_WindowHeight));
        m_Renderers.emplace_back(new ForwardRenderer(m_RenderPass, m_WindowWidth, m_WindowHeight));
        m_Renderers.emplace_back(new ImGuiRenderer(m_RenderPass, m_WindowWidth, m_WindowHeight));
    }

    void RenderManager::createRenderPass() {
        const RenderPassInfo renderPassInfo{ m_Renderer->getYzSwapchain()->getImageFormat() };
        m_RenderPass = new YzVkRenderPass(renderPassInfo);
    }

    void RenderManager::createFrameBuffers() {
        VkFormat depthFormat = VkUtil::findDepthFormat();
        m_DepthBuffer = YzVkImage::createDepthStencilBuffer(m_WindowWidth, m_WindowHeight, depthFormat);

        FramebufferInfo framebufferInfo;
        framebufferInfo.type = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_RenderPass;
        framebufferInfo.width = m_WindowWidth;
        framebufferInfo.height = m_WindowHeight;
        framebufferInfo.layers = 1;

        for (uint32_t i = 0; i < m_Renderer->getYzSwapchain()->getImageViewSize(); i++) {
            framebufferInfo.attachments = { m_Renderer->getYzSwapchain()->getImageView(i), m_DepthBuffer->getImageView() };
            m_FrameBuffers.push_back(new YzVkFramebuffer(framebufferInfo));
        }
    }

    void RenderManager::createCommandBuffers() {
        m_CommandBuffers.resize(m_FrameBuffers.size());

        for (unsigned int i = 0; i < m_CommandBuffers.size(); i++) {
            m_CurrentBufferID = i;

            m_CommandBuffers[i] = new YzVkCommandBuffer();
        }
    }

    void RenderManager::onResize() {
        // CleanUp
        {
            for (auto commandBuffer : m_CommandBuffers) {
                delete commandBuffer;
            }
            m_CommandBuffers.clear();

            for (auto frameBuffer : m_FrameBuffers) {
                delete frameBuffer;
            }
            m_FrameBuffers.clear();

            delete m_DepthBuffer;
            delete m_RenderPass;
        }

        m_WindowWidth =  m_Renderer->getYzSwapchain()->getExtent().width;
        m_WindowHeight = m_Renderer->getYzSwapchain()->getExtent().height;
        createRenderPass();
        createFrameBuffers();
        createCommandBuffers();

        for (auto renderer : m_Renderers) {
            renderer->onResize(m_RenderPass, m_WindowWidth, m_WindowHeight);
        }
    }
}