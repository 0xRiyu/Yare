#include "Platform/Vulkan/Vk_Pipeline.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Core/Yzh.h"

namespace Yarezo {
    namespace Graphics {
        YzVkPipeline::YzVkPipeline() {
        }

        YzVkPipeline::~YzVkPipeline() {
        }

        void YzVkPipeline::init(PipelineInfo& pipelineInfo) {
            // A descriptor is a special opaque shader variable that shaders use to access buffer and image
            // resources in an indirect fashion. It can be thought of as a "pointer" to a resource.
            // The layout is used to describe the content of a list of descriptor sets
            createDescriptorSetLayout();

            // Pipeline yay
            createGraphicsPipeline(pipelineInfo);

            // Descriptor sets can't be created directly, they must be allocated from a pool like command buffers. We create those here.
            createDescriptorPool(pipelineInfo);
        }

        void YzVkPipeline::cleanUp() {
            if (m_DescriptorSetLayout) {
                vkDestroyDescriptorSetLayout(YzVkDevice::instance()->getDevice(), m_DescriptorSetLayout, nullptr);
            }
            if (m_PipelineLayout) {
                vkDestroyPipelineLayout(YzVkDevice::instance()->getDevice(), m_PipelineLayout, nullptr);
            }
            if (m_GraphicsPipeline) {
                vkDestroyPipeline(YzVkDevice::instance()->getDevice(), m_GraphicsPipeline, nullptr);
            }
            if (m_DescriptorPool) {
                vkDestroyDescriptorPool(YzVkDevice::instance()->getDevice(), m_DescriptorPool, nullptr);
            }
        }

        void YzVkPipeline::setActive(const YzVkCommandBuffer& commandBuffer) {
            vkCmdBindPipeline(commandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
        }

        void YzVkPipeline::createDescriptorSetLayout() {

            VkDescriptorSetLayoutBinding viewUboLayoutBinding = {};
            viewUboLayoutBinding.binding = 0;
            viewUboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            viewUboLayoutBinding.descriptorCount = 1;
            // We are only using this in the vertex shader
            viewUboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            VkDescriptorSetLayoutBinding dynamicUboLayoutBinding = {};
            dynamicUboLayoutBinding.binding = 1;
            dynamicUboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            dynamicUboLayoutBinding.descriptorCount = 1;
            // We are only using this in the vertex shader
            dynamicUboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
            samplerLayoutBinding.binding = 2;
            samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerLayoutBinding.descriptorCount = MAX_NUM_TEXTURES;
            samplerLayoutBinding.pImmutableSamplers = nullptr;
            samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

            std::array<VkDescriptorSetLayoutBinding, 3> bindings = { viewUboLayoutBinding, dynamicUboLayoutBinding, samplerLayoutBinding };
            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            if (vkCreateDescriptorSetLayout(YzVkDevice::instance()->getDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan was unable to create a descriptor set layout.");
            }
        }

        void YzVkPipeline::createGraphicsPipeline(PipelineInfo& pipelineInfo) {
            auto bindingDescription = VulkanVertex::getBindingDescription();
            auto attributeDescriptions = VulkanVertex::getAttributeDescriptions();

            VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputInfo.vertexBindingDescriptionCount = 1;
            vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
            vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
            vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

            VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
            inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            inputAssembly.primitiveRestartEnable = VK_FALSE;

            VkViewport viewport = {};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float)pipelineInfo.swapchain->getExtent().width;
            viewport.height = (float)pipelineInfo.swapchain->getExtent().height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            VkRect2D scissor = {};
            scissor.offset = { 0,0 };
            scissor.extent = pipelineInfo.swapchain->getExtent();

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
            rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
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
            depthStencil.depthTestEnable = VK_TRUE;
            depthStencil.depthWriteEnable = VK_TRUE;
            depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
            depthStencil.depthBoundsTestEnable = VK_FALSE;
            depthStencil.stencilTestEnable = VK_FALSE;
            depthStencil.minDepthBounds = 0.0f; // Optional
            depthStencil.maxDepthBounds = 1.0f; // Optional
            depthStencil.front = {}; // Optional
            depthStencil.back = {}; // Optional

            VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
            colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            colorBlendAttachment.blendEnable = VK_FALSE;

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

            VkPushConstantRange pushConstantRange = {};
            pushConstantRange.offset = 0;
            pushConstantRange.size = sizeof(int);
            pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

            VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = 1;
            pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
            pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
            pipelineLayoutInfo.pushConstantRangeCount = 1;

            if (vkCreatePipelineLayout(YzVkDevice::instance()->getDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan Pipeline Layout was unable to be created.");
            }

            VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
            pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineCreateInfo.stageCount = pipelineInfo.shader->getStageCount();
            pipelineCreateInfo.pStages = pipelineInfo.shader->getShaderStages();
            pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
            pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
            pipelineCreateInfo.pViewportState = &viewportState;
            pipelineCreateInfo.pRasterizationState = &rasterizer;
            pipelineCreateInfo.pMultisampleState = &multisampling;
            pipelineCreateInfo.pDepthStencilState = &depthStencil;
            pipelineCreateInfo.pColorBlendState = &colorBlending;
            pipelineCreateInfo.pDynamicState = nullptr; // Optional
            pipelineCreateInfo.layout = m_PipelineLayout;
            pipelineCreateInfo.renderPass = pipelineInfo.renderpass->getRenderPass();
            pipelineCreateInfo.subpass = 0;
            pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

            if (vkCreateGraphicsPipelines(YzVkDevice::instance()->getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan failed to create the graphics pipeline.");
            }


        }
        void YzVkPipeline::createDescriptorPool(PipelineInfo& pipelineInfo) {
            size_t swapchainImagesSize = pipelineInfo.swapchain->getImagesSize();

            std::array<VkDescriptorPoolSize, 3> poolSizes = {};
            poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            poolSizes[0].descriptorCount = static_cast<uint32_t>(1);
            poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            poolSizes[1].descriptorCount = static_cast<uint32_t>(1);
            poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_NUM_TEXTURES);

            VkDescriptorPoolCreateInfo poolInfo = {};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
            poolInfo.pPoolSizes = poolSizes.data();
            poolInfo.maxSets = static_cast<uint32_t>(2);

            if (vkCreateDescriptorPool(YzVkDevice::instance()->getDevice(), &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan creation of descriptor pool failed.");
            }

        }
    }
}
