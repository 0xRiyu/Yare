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

            // m_DescriptorSets.resize(descriptorSetInfo.descriptorSetCount);

            //This wont need to be cleaned up because it is auto cleaned up when the pool is destroyed
            if (vkAllocateDescriptorSets(YzVkDevice::instance()->getDevice(), &allocInfo, &m_DescriptorSets) != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan was unable to allocate descriptor sets.");
            }

        }

        void YzVkDescriptorSet::update(std::vector<BufferInfo>& newBufferInfo) {

            std::vector<VkWriteDescriptorSet> descriptorWrites = {};

            for (const auto& bufferInfo : newBufferInfo) {
                if (bufferInfo.type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
                    VkDescriptorImageInfo imageInfo = {};
                    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo.imageView = bufferInfo.imageView;
                    imageInfo.sampler = bufferInfo.imageSampler;

                    VkWriteDescriptorSet descriptorWrite = {};
                    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrite.dstSet = m_DescriptorSets;
                    descriptorWrite.dstBinding = bufferInfo.binding;
                    descriptorWrite.descriptorType = bufferInfo.type;
                    descriptorWrite.descriptorCount = 1;
                    descriptorWrite.pImageInfo = &imageInfo;

                    descriptorWrites.push_back(descriptorWrite);
                }
                else {
                    VkDescriptorBufferInfo bInfo = {};
                    bInfo.buffer = bufferInfo.buffer;
                    bInfo.offset = bufferInfo.offset;
                    bInfo.range = bufferInfo.size;

                    VkWriteDescriptorSet descriptorWrite = {};
                    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrite.dstSet = m_DescriptorSets;
                    descriptorWrite.dstBinding = bufferInfo.binding;
                    descriptorWrite.descriptorType = bufferInfo.type;
                    descriptorWrite.descriptorCount = 1;
                    descriptorWrite.pBufferInfo = &bInfo;

                    descriptorWrites.push_back(descriptorWrite);
                }
            }
            vkUpdateDescriptorSets(YzVkDevice::instance()->getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

        }
    }
}
