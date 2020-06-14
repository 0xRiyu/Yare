#ifndef YARE_RENDERER_H
#define YARE_RENDERER_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/Instance.h"
#include "Graphics/Vulkan/Devices.h"
#include "Graphics/Vulkan/Swapchain.h"
#include "Graphics/Vulkan/CommandBuffer.h"
#include "Graphics/Vulkan/Semaphore.h"

namespace Yare::Graphics {

    class YzVkRenderer {
    public:
        YzVkRenderer(size_t width, size_t height);
        ~YzVkRenderer();

        void onResize(size_t width, size_t height);
        bool begin();
        bool present(YzVkCommandBuffer* cmdBuffer);

        std::shared_ptr<YzVkSwapchain> getYzSwapchain() const { return m_YzSwapchain; }

    private:
        void init(size_t width, size_t height);
        void submitGfxQueue(YzVkCommandBuffer* cmdBuffer, bool waitFence);

        const int MAX_FRAMES_IN_FLIGHT = 2;

        YzVkInstance*                     m_Instance;
        YzVkDevice*                       m_Device;
        std::shared_ptr<YzVkSwapchain>    m_YzSwapchain;

        std::vector<YzVkSemaphore>        m_ImageAvailableSemaphores;
        std::vector<YzVkSemaphore>        m_RenderFinishedSemaphores;
        size_t m_CurrentFrame = 0;
    };

}


#endif // YARE_RENDERER_H
