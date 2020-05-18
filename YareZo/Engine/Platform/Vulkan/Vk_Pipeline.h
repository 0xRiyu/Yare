#ifndef YAREZO_VK_PIPELINE_H
#define YAREZO_VK_PIPELINE_H

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_Renderpass.h"
#include "Platform/Vulkan/Vk_Swapchain.h"
#include "Platform/Vulkan/Vk_CommandBuffer.h"
#include "Platform/Vulkan/Vk_Shader.h"
#include "Core/DataStructures.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

#define MAX_NUM_TEXTURES 256

namespace Yarezo {
    namespace Graphics {

        struct VulkanVertex {
            Vertex vertex;

            static VkVertexInputBindingDescription getBindingDescription() {
                VkVertexInputBindingDescription bindingDescription = {};
                bindingDescription.binding = 0;
                bindingDescription.stride = sizeof(VulkanVertex);
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                return bindingDescription;
            }

            static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
                std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};
                attributeDescriptions[0].binding = 0;
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[0].offset = offsetof(VulkanVertex, vertex) + offsetof(Vertex, pos);

                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[1].offset = offsetof(VulkanVertex, vertex) + offsetof(Vertex, color);

                attributeDescriptions[2].binding = 0;
                attributeDescriptions[2].location = 2;
                attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[2].offset = offsetof(VulkanVertex, vertex) + offsetof(Vertex, texCoord);

                return attributeDescriptions;
            }
        };

        struct PipelineInfo {
            YzVkShader* shader;
            YzVkRenderPass* renderpass;
            YzVkSwapchain* swapchain;
            bool depthWriteEnable;
            bool depthTestEnable;
            VkCullModeFlags cullMode;
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            uint32_t maxObjects;
        };

        class YzVkPipeline {

        public:
            YzVkPipeline();
            ~YzVkPipeline();
            void init(PipelineInfo& pipelineInfo);
            void cleanUp();
            void setActive(const YzVkCommandBuffer& commandBuffer);

            const VkDescriptorPool& getDescriptorPool()            const { return m_DescriptorPool; }
            const VkDescriptorSetLayout& getDescriptorSetLayout()  const { return m_DescriptorSetLayout; }
            const VkPipelineLayout& getPipelineLayout()            const { return m_PipelineLayout; }
            const VkPipeline& getPipeline()                        const { return m_GraphicsPipeline; }

        private:
            void createDescriptorSetLayout();
            void createGraphicsPipeline();
            void createDescriptorPool();

        private:
            PipelineInfo m_PipelineInfo;

            VkDescriptorPool m_DescriptorPool;
            VkDescriptorSetLayout m_DescriptorSetLayout;
            VkPipelineLayout m_PipelineLayout;
            VkPipeline m_GraphicsPipeline;

        };

    }
}

#endif //YAREZO_VK_PIPELINE_H
