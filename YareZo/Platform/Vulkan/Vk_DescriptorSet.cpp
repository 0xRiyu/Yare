#include "Platform/Vulkan/Vk_DescriptorSet.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Utilities/YzLogger.h"

namespace Yarezo {
	namespace Graphics {
		YzVkDescriptorSet::YzVkDescriptorSet() {

		}

		YzVkDescriptorSet::~YzVkDescriptorSet() {
		}

		void YzVkDescriptorSet::init(const DescriptorSetInfo& descriptorSetInfo) {

			std::vector<VkDescriptorSetLayout> layouts(descriptorSetInfo.descriptorSetCount, descriptorSetInfo.pipeline->getDescriptorSetLayout());
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = descriptorSetInfo.pipeline->getDescriptorPool();
			allocInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSetInfo.descriptorSetCount);
			allocInfo.pSetLayouts = layouts.data();

			m_DescriptorSets.resize(descriptorSetInfo.descriptorSetCount);

			//This wont need to be cleaned up because it is auto cleaned up when the pool is destroyed
			if (vkAllocateDescriptorSets(YzVkDevice::instance()->getDevice(), &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS) {
				YZ_CRITICAL("Vulkan was unable to allocate descriptor sets.");
			}

		}

		void YzVkDescriptorSet::update(BufferInfo& newBufferInfo) {
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = newBufferInfo.buffer;
			bufferInfo.offset = newBufferInfo.offset;
			bufferInfo.range = newBufferInfo.size;

			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[newBufferInfo.binding];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr; // Optional
			descriptorWrite.pTexelBufferView = nullptr; // Optional
			vkUpdateDescriptorSets(YzVkDevice::instance()->getDevice(), 1, &descriptorWrite, 0, nullptr);
		}
	}
}