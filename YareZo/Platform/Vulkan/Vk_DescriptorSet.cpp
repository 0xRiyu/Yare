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

			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = newBufferInfo.imageView;
			imageInfo.sampler = newBufferInfo.imageSampler;

			std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = m_DescriptorSets[newBufferInfo.binding];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = m_DescriptorSets[newBufferInfo.binding];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(YzVkDevice::instance()->getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}
}