#ifndef YAREZO_VK_COMMANDBUFFER_H
#define YAREZO_VK_COMMANDBUFFER_H

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_CommandPool.h"


namespace Yarezo {
	namespace Graphics {
		class YzVkCommandBuffer {
		public:
			YzVkCommandBuffer();
			~YzVkCommandBuffer();

			void init();
			void cleanUp();
			void beginRecording();
			void endRecording();
			void submitGfxQueue(VkPipelineStageFlags flags, VkSemaphore waitSemaphore, VkSemaphore signalSemaphore, bool waitFence);

			const VkCommandBuffer& getCommandBuffer() const { return m_CommandBuffer; }
			const VkFence& getFence() const { return m_Fence; }
		private:
			VkCommandBuffer m_CommandBuffer;
			VkFence m_Fence;

		};
	}
}


#endif //YAREZO_VK_COMMANDBUFFER_H