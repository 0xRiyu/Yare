#include "Graphics/Vulkan/Vk_Renderer.h"
#include "Core/Yzh.h"

namespace Yarezo::Graphics {

    YzVkRenderer::YzVkRenderer(size_t width, size_t height) {
        init(width, height);
    }

    YzVkRenderer::~YzVkRenderer() {
        m_ImageAvailableSemaphores.clear();
        m_RenderFinishedSemaphores.clear();

        m_YzSwapchain.reset();
        m_Instance->cleanUp();

        YzVkDevice::release();

        delete m_Instance;
    }

    void YzVkRenderer::init(size_t width, size_t height) {
        m_Instance = new YzVkInstance();
        m_Instance->init();
        // Create our static device singleton
        m_Device = YzVkDevice::instance();

        m_Instance->createCommandPool();

        // Create a swapchain, a swapchain is responsible for maintaining the images
        // that will be presented to the user.
        m_YzSwapchain = std::make_shared<YzVkSwapchain>(width, height);

        m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    }

    void YzVkRenderer::onResize(size_t width, size_t height) {
        YzVkDevice::instance()->waitIdle();
        m_YzSwapchain->onResize(width, height);
    }

    bool YzVkRenderer::begin() {
        VkResult result = m_YzSwapchain->acquireNextImage(m_ImageAvailableSemaphores[m_CurrentFrame].getSemaphore());

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            return false;
        }
        else if (result != VK_SUCCESS) {
            YZ_ERROR("Vulkan failed to aquire a swapchain image.");
        }
        return true;
    }

    bool YzVkRenderer::present(YzVkCommandBuffer* cmdBuffer) {

        cmdBuffer->submitGfxQueue(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                  m_ImageAvailableSemaphores[m_CurrentFrame].getSemaphore(),
                                  m_RenderFinishedSemaphores[m_CurrentFrame].getSemaphore(),
                                  true);

        VkResult result = m_YzSwapchain->present(m_RenderFinishedSemaphores[m_CurrentFrame].getSemaphore());

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            return false;
        }
        else if (result != VK_SUCCESS) {
            YZ_ERROR("Vulkan failed to present a swapchain image.");
        }

        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        return true;
    }
}
