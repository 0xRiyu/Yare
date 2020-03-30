#ifndef YAREZO_VK_RENDERER_H
#define YAREZO_VK_RENDERER_H

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_Instance.h"
#include "Platform/Vulkan/Vk_Swapchain.h"
#include "Platform/Vulkan/Vk_DescriptorSet.h"
#include "Platform/Vulkan/Vk_CommandBuffer.h"
#include "Platform/Vulkan/Vk_Semaphore.h"


namespace Yarezo {
	namespace Graphics {

		class YzVkRenderer {
		public:
			YzVkRenderer();
			~YzVkRenderer();

			void init();
			void createSemaphores();
			void recreateSwapchain();
			bool begin();
			bool present(YzVkCommandBuffer* cmdBuffer);

			std::shared_ptr<YzVkSwapchain> getYzSwapchain() const { return m_YzSwapchain; }

		private:

			const int MAX_FRAMES_IN_FLIGHT = 2;

			YzVkInstance* m_YzInstance;
			std::shared_ptr<YzVkSwapchain> m_YzSwapchain;

			std::vector<YzVkSemaphore>        m_ImageAvailableSemaphores;
			std::vector<YzVkSemaphore>        m_RenderFinishedSemaphores;
			size_t m_CurrentFrame = 0;
		};



	}
}

#endif // !YAREZO_VK_RENDERER_H
