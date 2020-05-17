#ifndef YAREZO_VK_DESCRIPTORSET_H
#define YAREZO_VK_DESCRIPTORSET_H

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_Pipeline.h"

#include <vector>

namespace Yarezo {
    namespace Graphics {

        struct DescriptorSetInfo {
            YzVkPipeline* pipeline;
            size_t descriptorSetCount;
        };

        struct BufferInfo {
            VkBuffer buffer;
            VkDescriptorType type;
            VkImageView imageView;
            VkSampler imageSampler;
            uint32_t offset;
            uint32_t size;
            int binding;
            uint32_t descriptorCount;
        };

        class YzVkDescriptorSet {
        public:
            YzVkDescriptorSet();
            ~YzVkDescriptorSet();

            void init(const DescriptorSetInfo& descriptorSetInfo);
            void update(std::vector<BufferInfo>& newBufferInfo);

            const VkDescriptorSet& getDescriptorSet(unsigned int index) const { return m_DescriptorSets; }

        private:
           VkDescriptorSet m_DescriptorSets;

        };
    }
}



#endif //YAREZO_VK_DESCRIPTORSET_H
