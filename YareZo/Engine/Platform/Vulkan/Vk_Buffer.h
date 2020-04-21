#ifndef YAREZO_VK_BUFFER_H
#define YAREZO_VK_BUFFER_H

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_CommandBuffer.h"

namespace Yarezo {
	namespace Graphics {
		class YzVkBuffer {
		public:
			YzVkBuffer();
			YzVkBuffer(VkBufferUsageFlags usageFlags, size_t size, const void* data);
			~YzVkBuffer();

			void init(VkBufferUsageFlags usageFlags, size_t size, const void* data);
			void cleanUp();
			void setData(size_t size, const void* data);
			void bind(const YzVkCommandBuffer& commandBuffer);
			const VkBuffer& getBuffer() const { return m_Buffer; }

		private:
			bool mapMemory(VkDeviceSize offset, VkDeviceSize size);
			void unmapMemory();

		private:
			VkBuffer m_Buffer;
			VkBufferUsageFlags m_Usage;
			VkDeviceMemory m_BufferMemory;
			VkDeviceSize m_Size = 0;
			void* m_MappedData = nullptr;
		};
	}
}

#endif //YAREZO_VK_BUFFER_H