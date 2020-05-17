#ifndef YAREZO_FORWARD_RENDERER_H
#define YAREZO_FORWARD_RENDERER_H

#include "Graphics/Renderers/YzRenderer.h"

#include "Core/DataStructures.h"

#include "Platform/Vulkan/Vk_Instance.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Platform/Vulkan/Vk_Swapchain.h"
#include "Platform/Vulkan/Vk_Renderpass.h"
#include "Platform/Vulkan/Vk_Pipeline.h"
#include "Platform/Vulkan/Vk_Framebuffer.h"
#include "Platform/Vulkan/Vk_CommandPool.h"
#include "Platform/Vulkan/Vk_Buffer.h"
#include "Platform/Vulkan/Vk_DescriptorSet.h"
#include "Platform/Vulkan/Vk_CommandBuffer.h"
#include "Platform/Vulkan/Vk_Semaphore.h"
#include "Platform/Vulkan/Vk_Image.h"
#include "Platform/Vulkan/Vk_Renderer.h"
#define MAX_OBJECTS 2048

namespace Yarezo::Graphics {


    class ForwardRenderer  : public YzRenderer {
    public:
        ForwardRenderer();
        ~ForwardRenderer() override;

        virtual void init() override;
        virtual void renderScene() override;
        virtual void submitModel(Model* model, const glm::mat4& transform) override;
        virtual void present() override;
        virtual void begin() override;
        virtual void end() override;

        void cleanupSwapChain();
        void waitIdle();
        void recreateSwapChain();

        void createGraphicsPipeline();
        void createFrameBuffers();
        void createDescriptorSets();
        void createCommandBuffers();
        void prepareUniformBuffers();
        void updateUniformBuffers(uint32_t index);

    private:
        // TODO Move this out of here
        Model* m_ChaletModel;
        Model* m_CubeModel;
        Model* m_VikingModel;
        glm::mat4 m_ModelPos;
        size_t m_DynamicAlignment = 0;

        YzVkRenderer*                       m_Renderer;
        YzVkPipeline*                       m_Pipeline;
        YzVkRenderPass*                     m_RenderPass;
        YzVkDescriptorSet*                  m_DescriptorSet;
        YzVkImage*                          m_DepthBuffer;
        std::vector<YzVkCommandBuffer*>     m_CommandBuffers;
        std::vector<YzVkFramebuffer*>       m_FrameBuffers;
        //        std::vector<YzVkBuffer*>            m_UniformBuffers;

        uint32_t m_WindowWidth, m_WindowHeight;

        uint32_t m_CurrentBufferID;

        struct UniformBuffers {
            YzVkBuffer* view;
            YzVkBuffer* dynamic;
        } m_UniformBuffers;

        UboDataDynamic m_UboDynamicData;
};

}

#endif // YAREZO_FORWARD_RENDERER_H
