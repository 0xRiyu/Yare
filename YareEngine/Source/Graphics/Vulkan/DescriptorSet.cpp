#include "Graphics/Vulkan/DescriptorSet.h"

#include "Graphics/Vulkan/Devices.h"
#include "Utilities/Logger.h"

namespace Yare::Graphics {

    DescriptorSet::DescriptorSet() {}

    DescriptorSet::~DescriptorSet() {}

    void DescriptorSet::init(const DescriptorSetInfo& descriptorSetInfo) {
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorSetInfo.pipeline->getDescriptorPool();
        allocInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSetInfo.descriptorSetCount);
        allocInfo.pSetLayouts = &descriptorSetInfo.pipeline->getDescriptorSetLayout();

        // This wont need to be cleaned up because it is auto cleaned up when the pool is destroyed
        auto res = vkAllocateDescriptorSets(Devices::instance()->getDevice(), &allocInfo, &m_DescriptorSets);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan was unable to allocate descriptor sets.");
        }
    }

    void DescriptorSet::update(std::vector<BufferInfo>& newBufferInfo) {
        std::vector<VkWriteDescriptorSet> descriptorWrites = {};

        // Texture array reference; http://kylehalladay.com/blog/tutorial/vulkan/2018/01/28/Textue-Arrays-Vulkan.html
        // 32 is a magic number that we use for the max number of buffer infos
        std::vector<VkDescriptorBufferInfo> bInfo;
        std::vector<VkDescriptorImageInfo>  imageInfo;
        bInfo.resize(32);
        imageInfo.resize(std::min(256u, Devices::instance()->getGPUProperties().limits.maxPerStageDescriptorSamplers));

        uint32_t bufferIndex = 0;

        for (const auto& bufferInfo : newBufferInfo) {
            if (bufferInfo.type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
                for (int i = 0; i < bufferInfo.imageViews.size(); ++i) {
                    imageInfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo[i].imageView = bufferInfo.imageViews[i];
                    imageInfo[i].sampler = bufferInfo.imageSamplers[i];
                }
                VkWriteDescriptorSet descriptorWrite = {};
                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = m_DescriptorSets;
                descriptorWrite.dstBinding = bufferInfo.binding;
                descriptorWrite.dstArrayElement = 0;
                descriptorWrite.descriptorType = bufferInfo.type;
                descriptorWrite.descriptorCount = bufferInfo.descriptorCount;
                descriptorWrite.pImageInfo = imageInfo.data();

                descriptorWrites.push_back(descriptorWrite);
            } else {
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
        vkUpdateDescriptorSets(Devices::instance()->getDevice(), static_cast<uint32_t>(descriptorWrites.size()),
                               descriptorWrites.data(), 0, nullptr);
    }
}  // namespace Yare::Graphics
