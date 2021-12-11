#ifndef YARE_PIPELINE_H
#define YARE_PIPELINE_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/Renderpass.h"
#include "Graphics/Vulkan/Swapchain.h"
#include "Graphics/Vulkan/CommandBuffer.h"
#include "Graphics/Vulkan/Shader.h"
#include "Core/DataStructures.h"

#define MAX_NUM_TEXTURES 16

namespace Yare::Graphics {

    struct PipelineInfo {
        Shader* shader;
        RenderPass* renderpass;
        bool depthWriteEnable;
        bool depthTestEnable;
        VkCullModeFlags cullMode;
        std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
        std::vector<VkVertexInputAttributeDescription> vertexInputAttributes;
        VkVertexInputBindingDescription bindingDescription;
        std::vector<VkDynamicState> dynamicStates;
        uint32_t maxObjects;
        size_t width;
        size_t height;
        VkPushConstantRange pushConstants;
        bool colorBlendingEnabled = false;
    };

    class Pipeline {

    public:
        Pipeline();
        ~Pipeline();
        void init(PipelineInfo& pipelineInfo);
        void setActive(const CommandBuffer& commandBuffer);

        const VkDescriptorPool&      getDescriptorPool()       const { return m_DescriptorPool; }
        const VkDescriptorSetLayout& getDescriptorSetLayout()  const { return m_DescriptorSetLayout; }
        const VkPipelineLayout&      getPipelineLayout()       const { return m_PipelineLayout; }
        const VkPipeline&            getPipeline()             const { return m_GraphicsPipeline; }

    private:
        void createDescriptorSetLayout();
        void createGraphicsPipeline();
        void createDescriptorPool();

    private:
        PipelineInfo m_PipelineInfo;

        VkDescriptorPool      m_DescriptorPool        = VK_NULL_HANDLE;
        VkDescriptorSetLayout m_DescriptorSetLayout   = VK_NULL_HANDLE;
        VkPipelineLayout      m_PipelineLayout        = VK_NULL_HANDLE;
        VkPipeline            m_GraphicsPipeline      = VK_NULL_HANDLE;

    };
}

#endif //YARE_PIPELINE_H
