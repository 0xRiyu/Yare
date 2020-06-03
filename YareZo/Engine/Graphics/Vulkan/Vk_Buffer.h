#ifndef YAREZO_VK_BUFFER_H
#define YAREZO_VK_BUFFER_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/Vk_CommandBuffer.h"
#include "Graphics/Vulkan/Vk_Utilities.h"

namespace Yarezo {
    namespace Graphics {
        class YzVkBuffer {
        public:
            YzVkBuffer();
            YzVkBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, size_t size, const void* data);
            ~YzVkBuffer();

            void init(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, size_t size, const void* data);
            void cleanUp();
            void setData(size_t size, const void* data, uint32_t offset = 0);
            void setDynamicData(size_t size, const void* data, uint32_t offset = 0);
            void bindIndex(YzVkCommandBuffer* commandBuffer, VkIndexType type);
            void bindVertex(YzVkCommandBuffer* commandBuffer, VkDeviceSize offset);
            void flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            bool mapMemory(VkDeviceSize offset, VkDeviceSize size);
            void unmapMemory();

            const VkBuffer& getBuffer() const { return m_Buffer; }
            void* getMappedData() const { return m_MappedData; }
            size_t getSize() const { return m_Size; }

        private:
            VkBuffer m_Buffer;
            VkBufferUsageFlags m_Usage;
            VkMemoryPropertyFlags m_Properties;
            VkDeviceMemory m_BufferMemory;
            size_t m_Size = 0;
            void* m_MappedData = nullptr;
        };
    }
}

#endif //YAREZO_VK_BUFFER_H
