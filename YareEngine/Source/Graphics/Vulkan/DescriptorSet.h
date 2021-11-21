#ifndef YARE_DESCRIPTORSET_H
#define YARE_DESCRIPTORSET_H

#include <vector>

#include "Graphics/Vulkan/Pipeline.h"
#include "Graphics/Vulkan/Vk.h"

namespace Yare {
    namespace Graphics {

        struct DescriptorSetInfo {
            Pipeline* pipeline;
            size_t    descriptorSetCount;
        };

        struct BufferInfo {
            VkBuffer         buffer;
            VkDescriptorType type;
            VkImageView      imageView;
            VkSampler        imageSampler;
            uint32_t         offset;
            uint32_t         size;
            int              binding;
            uint32_t         descriptorCount;
        };

        class DescriptorSet {
           public:
            DescriptorSet();
            ~DescriptorSet();

            void init(const DescriptorSetInfo& descriptorSetInfo);
            void update(std::vector<BufferInfo>& newBufferInfo);

            const VkDescriptorSet& getDescriptorSet(unsigned int index) const { return m_DescriptorSets; }

           private:
            VkDescriptorSet m_DescriptorSets;
        };
    }  // namespace Graphics
}  // namespace Yare
#endif  // YARE_DESCRIPTORSET_H
