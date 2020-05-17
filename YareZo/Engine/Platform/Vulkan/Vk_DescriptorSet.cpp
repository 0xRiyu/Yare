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

            VkDescriptorSetAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = descriptorSetInfo.pipeline->getDescriptorPool();
            allocInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSetInfo.descriptorSetCount);
            allocInfo.pSetLayouts = &descriptorSetInfo.pipeline->getDescriptorSetLayout();

            //This wont need to be cleaned up because it is auto cleaned up when the pool is destroyed
            if (vkAllocateDescriptorSets(YzVkDevice::instance()->getDevice(), &allocInfo, &m_DescriptorSets) != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan was unable to allocate descriptor sets.");
            }

        }

        void YzVkDescriptorSet::update(std::vector<BufferInfo>& newBufferInfo) {

            std::vector<VkWriteDescriptorSet> descriptorWrites = {};

            // Texture array reference; http://kylehalladay.com/blog/tutorial/vulkan/2018/01/28/Textue-Arrays-Vulkan.html

            // 32 is a magic number that we use for the max number of buffer infos
            std::vector<VkDescriptorBufferInfo> bInfo;
            std::vector<VkDescriptorImageInfo> imageInfo;
            bInfo.resize(32);
            imageInfo.resize(MAX_NUM_TEXTURES);

            uint32_t bufferIndex = 0;
            uint32_t imageIndex = 0;

            for (const auto& bufferInfo : newBufferInfo) {
                if (bufferInfo.type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
                    imageInfo[imageIndex].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo[imageIndex].imageView = bufferInfo.imageView;
                    imageInfo[imageIndex].sampler = bufferInfo.imageSampler;

                    VkWriteDescriptorSet descriptorWrite = {};
                    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrite.dstSet = m_DescriptorSets;
                    descriptorWrite.dstBinding = bufferInfo.binding;
                    descriptorWrite.dstArrayElement = 0;
                    descriptorWrite.descriptorType = bufferInfo.type;
                    descriptorWrite.descriptorCount = bufferInfo.descriptorCount;
                    descriptorWrite.pImageInfo = imageInfo.data();

                    descriptorWrites.push_back(descriptorWrite);
                    imageIndex++;
                }
                else {
                    bInfo[bufferIndex].buffer = bufferInfo.buffer;
                    bInfo[bufferIndex].offset = bufferInfo.offset;
                    bInfo[bufferIndex].range = bufferInfo.size;

                    VkWriteDescriptorSet descriptorWrite = {};
                    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrite.dstSet = m_DescriptorSets;
                    descriptorWrite.dstBinding = bufferInfo.binding;
                    descriptorWrite.descriptorType = bufferInfo.type;
                    descriptorWrite.descriptorCount = bufferInfo.descriptorCount;
                    descriptorWrite.pBufferInfo = &bInfo[bufferIndex];

                    descriptorWrites.push_back(descriptorWrite);
                    bufferIndex++;
                }
            }
            vkUpdateDescriptorSets(YzVkDevice::instance()->getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

        }
    }
}
