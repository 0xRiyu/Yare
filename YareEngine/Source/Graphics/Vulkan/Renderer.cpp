#include "Graphics/Vulkan/Renderer.h"
#include "Core/Yzh.h"

namespace Yare::Graphics {

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
        auto result = m_YzSwapchain->acquireNextImage(m_ImageAvailableSemaphores[m_CurrentFrame].getSemaphore());

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            return false;
        }
        else if (result != VK_SUCCESS) {
            YZ_ERROR("Vulkan failed to aquire a swapchain image.");
        }
        return true;
    }

    bool YzVkRenderer::present(YzVkCommandBuffer* cmdBuffer) {

        submitGfxQueue(cmdBuffer, true);

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

    void YzVkRenderer::submitGfxQueue(YzVkCommandBuffer* cmdBuffer, bool waitFence) {
        auto currentWaitSemaphore = m_ImageAvailableSemaphores[m_CurrentFrame].getSemaphore();
        auto currentSignalSemaphore = m_RenderFinishedSemaphores[m_CurrentFrame].getSemaphore();

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmdBuffer->getCommandBuffer();
        VkPipelineStageFlags flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submitInfo.pWaitDstStageMask = &flags;
        submitInfo.pWaitSemaphores = &currentWaitSemaphore;
        submitInfo.waitSemaphoreCount = (uint32_t)(currentWaitSemaphore ? 1 : 0);
        submitInfo.pSignalSemaphores = &currentSignalSemaphore;
        submitInfo.signalSemaphoreCount =  (uint32_t)(currentSignalSemaphore ? 1 : 0);
        submitInfo.pNext = VK_NULL_HANDLE;

        if (!waitFence) {
            vkQueueSubmit(m_Device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(m_Device->getGraphicsQueue());
        }
        else {
            auto fence = cmdBuffer->getFence();
            vkQueueSubmit(m_Device->getGraphicsQueue(), 1, &submitInfo, fence);
            vkWaitForFences(m_Device->getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
            vkResetFences(m_Device->getDevice(), 1, &fence);
        }
    }
}
