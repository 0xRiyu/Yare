#ifndef YAREZO_SKYBOX_RENDERER_H
#define YAREZO_SKYBOX_RENDERER_H

#include "Graphics/Renderers/YzRenderer.h"


#include "Core/DataStructures.h"

#include "Graphics/Vulkan/Vk_Instance.h"
#include "Graphics/Vulkan/Vk_Devices.h"
#include "Graphics/Vulkan/Vk_Swapchain.h"
#include "Graphics/Vulkan/Vk_Renderpass.h"
#include "Graphics/Vulkan/Vk_Pipeline.h"
#include "Graphics/Vulkan/Vk_Framebuffer.h"
#include "Graphics/Vulkan/Vk_CommandPool.h"
#include "Graphics/Vulkan/Vk_Buffer.h"
#include "Graphics/Vulkan/Vk_DescriptorSet.h"
#include "Graphics/Vulkan/Vk_CommandBuffer.h"
#include "Graphics/Vulkan/Vk_Semaphore.h"
#include "Graphics/Vulkan/Vk_Image.h"
#include "Graphics/Vulkan/Vk_Renderer.h"

#include "Graphics/Gui/VulkanImGui.h"

namespace Yarezo::Graphics {

    class SkyboxRenderer : public YzRenderer {
    public:
        SkyboxRenderer();
        ~SkyboxRenderer() override;

        virtual void init() override;
        virtual void renderScene() override;
        virtual void submitModel(Model* model, const glm::mat4& transform) override;
        virtual void present() override;
        virtual void begin() override;
        virtual void end() override;



    private:
        Model* m_SkyboxModel;
        YzVkPipeline* m_Pipeline;
        YzVkDescriptorSet* m_DescriptorSet;
        YzVkBuffer* m_UniformBuffer;
    };

}

#endif // YAREZO_SKYBOX_RENDERER_H
