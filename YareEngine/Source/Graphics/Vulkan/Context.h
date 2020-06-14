#ifndef YARE_VULKAN_CONTEXT_H
#define YARE_VULKAN_CONTEXT_H

#include "Graphics/Vulkan/Vk.h"
#include "Graphics/Vulkan/CommandPool.h"
#include "Graphics/Vulkan/Devices.h"
#include "Graphics/Vulkan/Swapchain.h"
#include "Graphics/Vulkan/CommandBuffer.h"
#include "Graphics/Vulkan/Semaphore.h"

namespace Yare::Graphics {

    class VulkanContext {
    public:
        VulkanContext(size_t width, size_t height);
        ~VulkanContext();

        void onResize(size_t width, size_t height);
        bool begin();
        bool present(CommandBuffer* cmdBuffer);

        const std::shared_ptr<Swapchain>&   getSwapchain()    const { return m_Swapchain; }
        const std::shared_ptr<CommandPool>& getCommandPool()  const { return m_CommandPool; }
        const VkInstance&                   getInstance()     const { return m_Instance; }
        const static VulkanContext*         getContext()            { return s_Context; }

    private:
        void init(size_t width, size_t height);
        void createInstance();
        void setupDebugMessenger();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();

        void submitGfxQueue(CommandBuffer* cmdBuffer, bool waitFence);

    private:
        VkInstance                        m_Instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT          m_DebugMessenger;

        static VulkanContext*             s_Context;
        Devices*                          m_Devices;
        std::shared_ptr<CommandPool>      m_CommandPool;
        std::shared_ptr<Swapchain>        m_Swapchain;

        std::vector<Semaphore>            m_ImageAvailableSemaphores;
        std::vector<Semaphore>            m_RenderFinishedSemaphores;
        size_t                            m_CurrentFrame = 0;

        const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif
        const int MAX_FRAMES_IN_FLIGHT = 2;

    };
}

#endif // YARE_VULKAN_CONTEXT_H
