#ifndef YAREZO_VK_RENDERER_H
#define YAREZO_VK_RENDERER_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/Vk_Instance.h"
#include "Graphics/Vulkan/Vk_Devices.h"
#include "Graphics/Vulkan/Vk_Swapchain.h"
#include "Graphics/Vulkan/Vk_DescriptorSet.h"
#include "Graphics/Vulkan/Vk_CommandBuffer.h"
#include "Graphics/Vulkan/Vk_Semaphore.h"


namespace Yarezo {
    namespace Graphics {

        class YzVkRenderer {
        public:
            YzVkRenderer();
            ~YzVkRenderer();

            void recreateSwapchain();
            bool begin();
            bool present(YzVkCommandBuffer* cmdBuffer);

            std::shared_ptr<YzVkSwapchain> getYzSwapchain() const { return m_YzSwapchain; }

        protected:
            void init();
            void createSemaphores();

        private:
            const int MAX_FRAMES_IN_FLIGHT = 1;

            YzVkInstance*                     m_Instance;
            YzVkDevice*                       m_Device;
            std::shared_ptr<YzVkSwapchain>    m_YzSwapchain;

            std::vector<YzVkSemaphore>        m_ImageAvailableSemaphores;
            std::vector<YzVkSemaphore>        m_RenderFinishedSemaphores;
            size_t m_CurrentFrame = 0;
        };

    }
}

#endif // YAREZO_VK_RENDERER_H
