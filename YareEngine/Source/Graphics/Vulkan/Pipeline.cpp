#include "Graphics/Vulkan/Pipeline.h"
#include "Graphics/Vulkan/Devices.h"
#include "Core/Yzh.h"

namespace Yare::Graphics {

    Pipeline::Pipeline() {
    }

    Pipeline::~Pipeline() {
        if (m_DescriptorSetLayout) {
            vkDestroyDescriptorSetLayout(Devices::instance()->getDevice(), m_DescriptorSetLayout, nullptr);
        }
        if (m_PipelineLayout) {
            vkDestroyPipelineLayout(Devices::instance()->getDevice(), m_PipelineLayout, nullptr);
        }
        if (m_GraphicsPipeline) {
            vkDestroyPipeline(Devices::instance()->getDevice(), m_GraphicsPipeline, nullptr);
        }
        if (m_DescriptorPool) {
            vkDestroyDescriptorPool(Devices::instance()->getDevice(), m_DescriptorPool, nullptr);
        }
    }

    void Pipeline::init(PipelineInfo& pipelineInfo) {
        m_PipelineInfo = pipelineInfo;
        // A descriptor is a special opaque shader variable that shaders use to access buffer and image
        // resources in an indirect fashion. It can be thought of as a "pointer" to a resource.
        // The layout is used to describe the content of a list of descriptor sets
        createDescriptorSetLayout();

        // Pipeline yay
        createGraphicsPipeline();

        // Descriptor sets can't be created directly, they must be allocated from a pool like command buffers. We create those here.
        createDescriptorPool();
    }

    void Pipeline::setActive(const CommandBuffer& commandBuffer) {
        vkCmdBindPipeline(commandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
    }

    void Pipeline::createDescriptorSetLayout() {

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(m_PipelineInfo.layoutBindings.size());
        layoutInfo.pBindings = m_PipelineInfo.layoutBindings.data();

        auto res = vkCreateDescriptorSetLayout(Devices::instance()->getDevice(),
                                               &layoutInfo, nullptr, &m_DescriptorSetLayout);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan was unable to create a descriptor set layout.");
        }
    }

    void Pipeline::createGraphicsPipeline() {

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &m_PipelineInfo.bindingDescription;

        vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)m_PipelineInfo.vertexInputAttributes.size();
        vertexInputInfo.pVertexAttributeDescriptions = m_PipelineInfo.vertexInputAttributes.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)m_PipelineInfo.width;
        viewport.height = (float)m_PipelineInfo.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0,0 };
        scissor.extent = { (uint32_t)m_PipelineInfo.width, (uint32_t)m_PipelineInfo.height };

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

        // If this is enabled, then fragments beyond near and far plains are clamped instead of discarded
        rasterizer.depthClampEnable = VK_FALSE;
        // If this is enabled, then we skip the rasterizer stage (we wont get output)
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        // Fill the polygon, just draw lines, or points set here
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = m_PipelineInfo.cullMode;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; //optional
        rasterizer.depthBiasClamp = 0.0f; //optional
        rasterizer.depthBiasSlopeFactor = 0.0f; //optional

        VkPipelineMultisampleStateCreateInfo multisampling = {};

        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f; //optional
        multisampling.pSampleMask = nullptr; //optional
        multisampling.alphaToCoverageEnable = VK_FALSE; //optional

        VkPipelineDepthStencilStateCreateInfo depthStencil = {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = m_PipelineInfo.depthTestEnable;
        depthStencil.depthWriteEnable = m_PipelineInfo.depthWriteEnable;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};

        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                              VK_COLOR_COMPONENT_G_BIT |
                                              VK_COLOR_COMPONENT_B_BIT |
                                              VK_COLOR_COMPONENT_A_BIT;
        if (m_PipelineInfo.colorBlendingEnabled) {
            colorBlendAttachment.blendEnable = VK_TRUE;
            colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
            colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
        } else {
            colorBlendAttachment.blendEnable = VK_FALSE;
        }

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;


        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
        pipelineLayoutInfo.pPushConstantRanges = &m_PipelineInfo.pushConstants;
        pipelineLayoutInfo.pushConstantRangeCount = 1;

        auto res = vkCreatePipelineLayout(Devices::instance()->getDevice(), &pipelineLayoutInfo,
                                          nullptr, &m_PipelineLayout);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan Pipeline Layout was unable to be created.");
        }

        VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.stageCount = m_PipelineInfo.shader->getStageCount();
        pipelineCreateInfo.pStages = m_PipelineInfo.shader->getShaderStages();
        pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
        pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
        pipelineCreateInfo.pViewportState = &viewportState;
        pipelineCreateInfo.pRasterizationState = &rasterizer;
        pipelineCreateInfo.pMultisampleState = &multisampling;
        pipelineCreateInfo.pDepthStencilState = &depthStencil;
        pipelineCreateInfo.pColorBlendState = &colorBlending;

        VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo = {};
        if (!m_PipelineInfo.dynamicStates.empty()) {
            pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            pipelineDynamicStateCreateInfo.pDynamicStates = m_PipelineInfo.dynamicStates.data();
            pipelineDynamicStateCreateInfo.dynamicStateCount = (uint32_t)m_PipelineInfo.dynamicStates.size();
            pipelineDynamicStateCreateInfo.flags = 0;
            pipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
        }

        pipelineCreateInfo.layout = m_PipelineLayout;
        pipelineCreateInfo.renderPass = m_PipelineInfo.renderpass->getRenderPass();
        pipelineCreateInfo.subpass = 0;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

        res = vkCreateGraphicsPipelines(Devices::instance()->getDevice(), VK_NULL_HANDLE, 1,
                                        &pipelineCreateInfo, nullptr, &m_GraphicsPipeline);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan failed to create a graphics pipeline.");
        }
    }

    void Pipeline::createDescriptorPool() {

        std::vector<VkDescriptorPoolSize> poolSizes(m_PipelineInfo.layoutBindings.size());

        int bindingIndex = 0;
        for (auto binding : m_PipelineInfo.layoutBindings) {
            poolSizes[bindingIndex].type = binding.descriptorType;
            poolSizes[bindingIndex].descriptorCount = binding.descriptorCount;
            bindingIndex++;
        }

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = m_PipelineInfo.maxObjects;

        auto res = vkCreateDescriptorPool(Devices::instance()->getDevice(), &poolInfo, nullptr, &m_DescriptorPool);
        if (res != VK_SUCCESS) {
            YZ_CRITICAL("Vulkan creation of descriptor pool failed.");
        }
    }
}
