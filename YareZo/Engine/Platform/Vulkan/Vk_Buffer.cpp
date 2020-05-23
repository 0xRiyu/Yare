#include "Platform/Vulkan/Vk_Buffer.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Utilities/YzLogger.h"

namespace Yarezo {
    namespace Graphics {
        YzVkBuffer::YzVkBuffer() {
        }

        YzVkBuffer::YzVkBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, size_t size, const void* data) {
            init(usageFlags, propertyFlags, size, data);
        }

        YzVkBuffer::~YzVkBuffer() {
            cleanUp();
        }

        void YzVkBuffer::cleanUp() {
            if (m_Buffer) {
                vkDestroyBuffer(YzVkDevice::instance()->getDevice(), m_Buffer, nullptr);
                if (m_BufferMemory) {
                    vkFreeMemory(YzVkDevice::instance()->getDevice(), m_BufferMemory, nullptr);
                }
            }
        }

        void YzVkBuffer::init(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, size_t size, const void* data) {
            m_Size = size;
            m_Properties = propertyFlags;
            m_Usage = usageFlags;

            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usageFlags;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(YzVkDevice::instance()->getDevice(), &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan was unable to create a buffer.");
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(YzVkDevice::instance()->getDevice(), m_Buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = VkUtil::findMemoryType(memRequirements.memoryTypeBits, propertyFlags);

            if (vkAllocateMemory(YzVkDevice::instance()->getDevice(), &allocInfo, nullptr, &m_BufferMemory) != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan failed to allocate buffer memory!");
            }

            vkBindBufferMemory(YzVkDevice::instance()->getDevice(), m_Buffer, m_BufferMemory, 0);

            if (data != nullptr) {
                setData(size, data);
            }
        }


        void YzVkBuffer::setData(size_t size, const void* data) {
            if (mapMemory(0, size)) {
                memcpy(m_MappedData, data, size);
                unmapMemory();
            }
            else {
                YZ_ERROR("Mapping failed - Not copying data to buffer");
            }
        }

        void YzVkBuffer::setDynamicData(size_t size, const void* data) {
            if (mapMemory(0, VK_WHOLE_SIZE)) {
                memcpy(m_MappedData, data, size);
                flush(size, 0);
                unmapMemory();
            }
            else {
                YZ_ERROR("Mapping failed - Not copying data to buffer");
            }
        }

        void YzVkBuffer::bind(const YzVkCommandBuffer& commandBuffer) {

            auto vertexBufferFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            if (m_Usage == vertexBufferFlags) {
                VkDeviceSize offset = 0;
                vkCmdBindVertexBuffers(commandBuffer.getCommandBuffer(), 0, 1, &m_Buffer, &offset);
            }
            auto indexBufferFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            if (m_Usage == indexBufferFlags) {
                vkCmdBindIndexBuffer(commandBuffer.getCommandBuffer(), m_Buffer, 0, VK_INDEX_TYPE_UINT32);
            }
        }

        bool YzVkBuffer::mapMemory(VkDeviceSize offset, VkDeviceSize size) {
            if (vkMapMemory(YzVkDevice::instance()->getDevice(), m_BufferMemory, offset, size, 0, &m_MappedData) != VK_SUCCESS) {
                YZ_ERROR("Failed to map buffer memory");
                return false;
            }
            return true;
        }

        void YzVkBuffer::unmapMemory() {
            vkUnmapMemory(YzVkDevice::instance()->getDevice(), m_BufferMemory);
        }

        void YzVkBuffer::flush(VkDeviceSize size, VkDeviceSize offset) {
            VkMappedMemoryRange mappedRange = {};
            mappedRange.sType =  VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            mappedRange.memory = m_BufferMemory;
            mappedRange.offset = offset;
            mappedRange.size = size;
            vkFlushMappedMemoryRanges(YzVkDevice::instance()->getDevice(), 1, &mappedRange);
        }
    }
}
