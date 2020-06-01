#include "Graphics/Renderers/ImGuiRenderer.h"
#include "Application/Application.h"
#include "Core/yzh.h"
#include "Core/Glfw.h"

#include "Application/GlobalSettings.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"

#include "Graphics/Vulkan/Vk_Instance.h"
#include "Graphics/Vulkan/Vk_Devices.h"
#include "Handlers/MouseHandler.h"

namespace Yarezo::Graphics {
    ImGuiRenderer::ImGuiRenderer() {
        ImGui::CreateContext();
    }

    ImGuiRenderer::~ImGuiRenderer() {
        ImGui::DestroyContext();
        delete m_Font;
        delete m_Pipeline;
        delete m_IndexBuffer;
        delete m_VertexBuffer;
    }

    void ImGuiRenderer::init(YzVkRenderPass* renderPass, uint32_t windowWidth, uint32_t windowHeight) {

        ImGui::StyleColorsDark();
        // Color scheme
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
        style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        // Dimensions
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        io.Fonts->AddFontFromFileTTF("../YareZo/Resources/Fonts/Cousine-Regular.ttf", 24.0f);

        initResources(renderPass);
    }

    void ImGuiRenderer::initResources(YzVkRenderPass* renderPass) {
        ImGuiIO& io = ImGui::GetIO();

        // Create font texture
        unsigned char* fontData;
        int texWidth, texHeight;
        io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);
        VkDeviceSize uploadSize = texWidth*texHeight * 4 * sizeof(char);

        m_Font = YzVkImage::createTexture2D(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, fontData);

        YzVkShader shader("../YareZo/Resources/Shaders", "gui.shader");

        PipelineInfo pipelineInfo = {};
        pipelineInfo.shader = &shader;
        pipelineInfo.renderpass = renderPass;
        pipelineInfo.cullMode = VK_CULL_MODE_NONE;
        pipelineInfo.depthTestEnable = VK_FALSE;
        pipelineInfo.depthWriteEnable = VK_FALSE;
        pipelineInfo.maxObjects = 2;
        pipelineInfo.width = (size_t)io.DisplaySize.x;
        pipelineInfo.height = (size_t)io.DisplaySize.y;
        pipelineInfo.colorBlendingEnabled = true;
        pipelineInfo.dynamicStates.emplace_back(VK_DYNAMIC_STATE_VIEWPORT);
        pipelineInfo.dynamicStates.emplace_back(VK_DYNAMIC_STATE_SCISSOR);
        pipelineInfo.pushConstants = {VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstBlock)};
        pipelineInfo.bindingDescription =  VkVertexInputBindingDescription{0, sizeof(ImDrawVert), VK_VERTEX_INPUT_RATE_VERTEX};

        pipelineInfo.vertexInputAttributes.emplace_back(VkVertexInputAttributeDescription{0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, pos)});
        pipelineInfo.vertexInputAttributes.emplace_back(VkVertexInputAttributeDescription{1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv)});
        pipelineInfo.vertexInputAttributes.emplace_back(VkVertexInputAttributeDescription{2, 0, VK_FORMAT_R8G8B8A8_UNORM, offsetof(ImDrawVert, col)});

        pipelineInfo.layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                              1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr});

        m_Pipeline = new YzVkPipeline();
        m_Pipeline->init(pipelineInfo);

        m_DescriptorSet = new YzVkDescriptorSet();
        m_DescriptorSet->init({m_Pipeline, 1});

        std::vector<BufferInfo> bufferInfos = {};
        BufferInfo bInfo;
        bInfo.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        bInfo.binding = 2;
        bInfo.descriptorCount = 1;
        bInfo.imageSampler = m_Font->getSampler();
        bInfo.imageView = m_Font->getImageView();
        bufferInfos.push_back(bInfo);

        m_DescriptorSet->update(bufferInfos);
    }

    void ImGuiRenderer::prepareScene() {
        resetCommandQueue();
        newFrame();
        ImGui::Begin("Settings", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav);
        std::string fpsStr = "FPS: " + std::to_string((int)GlobalSettings::instance()->fps);
        ImGui::Text(fpsStr.c_str());
        ImGui::Checkbox("Render models", &GlobalSettings::instance()->displayModels);
        ImGui::Checkbox("Display background", &GlobalSettings::instance()->displayBackground);
        ImGui::End();
        postFrame();
        updateBuffers();
    }

    void ImGuiRenderer::present(YzVkCommandBuffer* commandBuffer){
        ImGuiIO& io = ImGui::GetIO();

        vkCmdBindDescriptorSets(commandBuffer->getCommandBuffer(),
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                m_Pipeline->getPipelineLayout(),
                                0, 1, &m_DescriptorSet->getDescriptorSet(0),
                                0, nullptr);

        m_Pipeline->setActive(*commandBuffer);

        VkViewport dViewport = {};
        dViewport.width = io.DisplaySize.x;
        dViewport.height = io.DisplaySize.y;
        dViewport.minDepth = 0.0f;
        dViewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer->getCommandBuffer(), 0, 1, &dViewport);

        // UI scale and translate via push constants
        pushConstBlock.translate = glm::vec2(-1.0f);
        pushConstBlock.scale = glm::vec2(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
        vkCmdPushConstants(commandBuffer->getCommandBuffer(),
                           m_Pipeline->getPipelineLayout(),
                           VK_SHADER_STAGE_VERTEX_BIT, 0,
                           sizeof(PushConstBlock), &pushConstBlock);

        // Render commands
        ImDrawData* imDrawData = ImGui::GetDrawData();
        int32_t vertexOffset = 0;
        int32_t indexOffset = 0;

        if (imDrawData->CmdListsCount > 0) {
            m_IndexBuffer->bindIndex(*commandBuffer, VK_INDEX_TYPE_UINT16);
            m_VertexBuffer->bindVertex(*commandBuffer, 0);

            for (int32_t i = 0; i < imDrawData->CmdListsCount; i++) {
                const ImDrawList* cmd_list = imDrawData->CmdLists[i];
                for (int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++) {
                    const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[j];
                    VkRect2D scissorRect;
                    scissorRect.offset.x = std::max((int32_t)(pcmd->ClipRect.x), 0);
                    scissorRect.offset.y = std::max((int32_t)(pcmd->ClipRect.y), 0);
                    scissorRect.extent.width = (uint32_t)(pcmd->ClipRect.z - pcmd->ClipRect.x);
                    scissorRect.extent.height = (uint32_t)(pcmd->ClipRect.w - pcmd->ClipRect.y);
                    vkCmdSetScissor(commandBuffer->getCommandBuffer(), 0, 1, &scissorRect);

                    vkCmdDrawIndexed(commandBuffer->getCommandBuffer(),
                                     pcmd->ElemCount,
                                     1,
                                     indexOffset,
                                     vertexOffset,
                                     0);
                    indexOffset += pcmd->ElemCount;
                }
                vertexOffset += cmd_list->VtxBuffer.Size;
            }
        }
    }

    void ImGuiRenderer::onResize(YzVkRenderPass* renderPass, uint32_t newWidth, uint32_t newHeight){
        // Cleanup
        {
            delete m_Font;
            delete m_Pipeline;
        }
        init(renderPass, newWidth, newHeight);
    }

    void ImGuiRenderer::newFrame() {
        ImGui::NewFrame();
    }

    void ImGuiRenderer::postFrame() {
        // Render to generate draw buffers
        ImGui::EndFrame();
        ImGui::Render();
    }

    void ImGuiRenderer::updateBuffers() {
        ImDrawData* imDrawData = ImGui::GetDrawData();

        VkDeviceSize vertexBufferSize = imDrawData->TotalVtxCount * sizeof(ImDrawVert);
        VkDeviceSize indexBufferSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);

        if ((vertexBufferSize == 0) || (indexBufferSize == 0)) {
            return;
        }

        if (m_IndexBuffer == nullptr || m_IndexBuffer->getSize() != indexBufferSize) {
            delete m_IndexBuffer;
            m_IndexBuffer = new YzVkBuffer();
            m_IndexBuffer->init(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                                indexBufferSize, nullptr);
            m_IndexBuffer->mapMemory(0, indexBufferSize);
        }
        if (m_VertexBuffer == nullptr || m_VertexBuffer->getSize() != vertexBufferSize) {
            delete m_VertexBuffer;
            m_VertexBuffer = new YzVkBuffer();
            m_VertexBuffer->init(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                                 vertexBufferSize, nullptr);
            m_VertexBuffer->mapMemory(0, vertexBufferSize);
        }


        ImDrawVert* vtx_dst = reinterpret_cast<ImDrawVert*>(m_VertexBuffer->getMappedData());
        ImDrawIdx* idx_dst = reinterpret_cast<ImDrawIdx*>(m_IndexBuffer->getMappedData());

        for (int n = 0; n < imDrawData->CmdListsCount; n++) {
            const ImDrawList* cmd_list = imDrawData->CmdLists[n];
            memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
            memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
            vtx_dst += cmd_list->VtxBuffer.Size;
            idx_dst += cmd_list->IdxBuffer.Size;
        }

        m_IndexBuffer->flush();
        m_VertexBuffer->flush();
    }

}
