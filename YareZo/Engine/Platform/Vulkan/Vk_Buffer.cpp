#include "Platform/Vulkan/Vk_Buffer.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Platform/Vulkan/Vk_Utilities.h"
#include "Utilities/YzLogger.h"

namespace Yarezo {
	namespace Graphics {
		YzVkBuffer::YzVkBuffer() {
		}

      YzVkBuffer::YzVkBuffer(VkBufferUsageFlags usageFlags, size_t size, const void* data):
          m_Size(size) {
          init(usageFlags, size, data);
      }

      YzVkBuffer::~YzVkBuffer() {
      }

      void YzVkBuffer::cleanUp() {
          if (m_Buffer) {
              vkDestroyBuffer(YzVkDevice::instance()->getDevice(), m_Buffer, nullptr);
              if (m_BufferMemory) {
                  vkFreeMemory(YzVkDevice::instance()->getDevice(), m_BufferMemory, nullptr);
              }
          }
      }

      void YzVkBuffer::init(VkBufferUsageFlags usageFlags, size_t size, const void* data) {
          m_Size = size;

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
			allocInfo.memoryTypeIndex = VkUtil::findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			if (vkAllocateMemory(YzVkDevice::instance()->getDevice(), &allocInfo, nullptr, &m_BufferMemory) != VK_SUCCESS) {
				YZ_CRITICAL("Vulkan failed to allocate buffer memory!");
			}

			vkBindBufferMemory(YzVkDevice::instance()->getDevice(), m_Buffer, m_BufferMemory, 0);

			if (data != nullptr) {
				setData(size, data);
			}

			m_Usage = usageFlags;
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

		void YzVkBuffer::bind(const YzVkCommandBuffer& commandBuffer) {
			auto vertexBufferFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			auto indexBufferFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

			if (m_Usage == vertexBufferFlags) {
				VkDeviceSize offset = 0;
				vkCmdBindVertexBuffers(commandBuffer.getCommandBuffer(), 0, 1, &m_Buffer, &offset);
			}
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
	}
}
