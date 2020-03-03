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

			const VkCommandBuffer& getCommandBuffer() const { return m_CommandBuffer; }
		private:
			VkCommandBuffer m_CommandBuffer;

		};
	}
}


#endif //YAREZO_VK_COMMANDBUFFER_H