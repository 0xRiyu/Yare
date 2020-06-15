#include "Graphics/RenderManager.h"

#include "Graphics/Vulkan/Utilities.h"
#include "Graphics/Renderers/ForwardRenderer.h"
#include "Graphics/Renderers/ImGuiRenderer.h"
#include "Graphics/Renderers/SkyboxRenderer.h"

#include "Graphics/Window/GlfwWindow.h"

namespace Yare::Graphics {

    RenderManager::RenderManager(const std::shared_ptr<Window> window):
        m_WindowRef(window) {
        init();
    }

    RenderManager::~RenderManager() {
        Devices::instance()->waitIdle();

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

        delete m_VulkanContext;
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
        if (!m_VulkanContext->begin()) {
            onResize();
        }

        m_CurrentBufferID = m_VulkanContext->getSwapchain()->getCurrentImage();

        m_CommandBuffers[m_CurrentBufferID]->beginRecording();

        m_RenderPass->beginRenderPass(m_CommandBuffers[m_CurrentBufferID], m_FrameBuffers[m_CurrentBufferID]);
    }

    void RenderManager::end() {
        m_RenderPass->endRenderPass(m_CommandBuffers[m_CurrentBufferID]);

        m_CommandBuffers[m_CurrentBufferID]->endRecording();

        if (!m_VulkanContext->present(m_CommandBuffers[m_CurrentBufferID])) {
            onResize();
        }
    }

    void RenderManager::init() {
        auto props = m_WindowRef->getWindowProperties();
        m_WindowWidth =  props.width;
        m_WindowHeight = props.height;
        m_VulkanContext = new VulkanContext(m_WindowWidth, m_WindowHeight);
        createRenderPass();
        createFrameBuffers();
        createCommandBuffers();
        m_Renderers.emplace_back(new SkyboxRenderer(m_RenderPass, m_WindowWidth, m_WindowHeight));
        m_Renderers.emplace_back(new ForwardRenderer(m_RenderPass, m_WindowWidth, m_WindowHeight));
        m_Renderers.emplace_back(new ImGuiRenderer(m_RenderPass, m_WindowWidth, m_WindowHeight));
    }

    void RenderManager::createRenderPass() {
        RenderPassInfo renderPassInfo{};
        renderPassInfo.imageFormat = m_VulkanContext->getSwapchain()->getImageFormat();
        renderPassInfo.extent = VkExtent2D{m_WindowWidth, m_WindowHeight};
        m_RenderPass = new RenderPass(renderPassInfo);
    }

    void RenderManager::createFrameBuffers() {
        VkFormat depthFormat = VkUtil::findDepthFormat();
        m_DepthBuffer = Image::createDepthStencilBuffer(m_WindowWidth, m_WindowHeight, depthFormat);

        FramebufferInfo framebufferInfo;
        framebufferInfo.type = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_RenderPass;
        framebufferInfo.width = m_WindowWidth;
        framebufferInfo.height = m_WindowHeight;
        framebufferInfo.layers = 1;

        for (uint32_t i = 0; i < m_VulkanContext->getSwapchain()->getImageViewSize(); i++) {
            framebufferInfo.attachments = { m_VulkanContext->getSwapchain()->getImageView(i),
                                            m_DepthBuffer->getImageView() };
            m_FrameBuffers.push_back(new Framebuffer(framebufferInfo));
        }
    }

    void RenderManager::createCommandBuffers() {
        m_CommandBuffers.resize(m_FrameBuffers.size());

        for (unsigned int i = 0; i < m_CommandBuffers.size(); i++) {
            m_CurrentBufferID = i;

            m_CommandBuffers[i] = new CommandBuffer();
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
        m_WindowWidth =  m_WindowRef->getWindowProperties().width;
        m_WindowHeight = m_WindowRef->getWindowProperties().height;
        m_VulkanContext->onResize(m_WindowWidth, m_WindowHeight);
        createRenderPass();
        createFrameBuffers();
        createCommandBuffers();

        for (auto renderer : m_Renderers) {
            renderer->onResize(m_RenderPass, m_WindowWidth, m_WindowHeight);
        }
    }
}
