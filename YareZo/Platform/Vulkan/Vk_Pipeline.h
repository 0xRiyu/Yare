#ifndef YAREZO_VK_PIPELINE_H
#define YAREZO_VK_PIPELINE_H

#include "Platform/Vulkan/Vk.h"
#include "Platform/Vulkan/Vk_Renderpass.h"
#include "Platform/Vulkan/Vk_Swapchain.h"
#include "Platform/Vulkan/Vk_CommandBuffer.h"
#include "Platform/Vulkan/Vk_Shader.h"

#include <glm.hpp>
#include <array>
#include <gtc/matrix_transform.hpp>

namespace Yarezo {
    namespace Graphics {

        struct Vertex {
            glm::vec2 pos;
            glm::vec3 color;
            glm::vec2 texCoord;

            static VkVertexInputBindingDescription getBindingDescription() {
                VkVertexInputBindingDescription bindingDescription = {};
                bindingDescription.binding = 0;
                bindingDescription.stride = sizeof(Vertex);
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                return bindingDescription;
            }

            static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
                std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};
                attributeDescriptions[0].binding = 0;
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[0].offset = offsetof(Vertex, pos);

                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[1].offset = offsetof(Vertex, color);

                attributeDescriptions[2].binding = 0;
                attributeDescriptions[2].location = 2;
                attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

                return attributeDescriptions;
            }
        };

        struct PipelineInfo {
            YzVkShader* shader;
            YzVkRenderPass* renderpass;
            YzVkSwapchain* swapchain;

        };

        class YzVkPipeline {

        public:
            YzVkPipeline();
            ~YzVkPipeline();
            void init(PipelineInfo& pipelineInfo);
            void cleanUp();
            void cleanupDescSetLayout();
            void setActive(const YzVkCommandBuffer& commandBuffer);

            const VkDescriptorPool& getDescriptorPool()            const { return m_DescriptorPool; }
            const VkDescriptorSetLayout& getDescriptorSetLayout()  const { return m_DescriptorSetLayout; }
            const VkPipelineLayout& getPipelineLayout()            const { return m_PipelineLayout; }
            const VkPipeline& getPipeline()                        const { return m_GraphicsPipeline; }

        private:
            void createDescriptorSetLayout();
            void createGraphicsPipeline(PipelineInfo& pipelineInfo);
            void createDescriptorPool(PipelineInfo& pipelineInfo);

        private:
            VkDescriptorPool m_DescriptorPool;
            VkDescriptorSetLayout m_DescriptorSetLayout;
            VkPipelineLayout m_PipelineLayout;
            VkPipeline m_GraphicsPipeline;

        };

    }
}

#endif //YAREZO_VK_PIPELINE_H