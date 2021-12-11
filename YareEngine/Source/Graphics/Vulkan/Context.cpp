#include "Graphics/Vulkan/Context.h"
#include "Application/Application.h"
#include "Utilities/Logger.h"
#include "Core/Glfw.h"

namespace Yare::Graphics {

    VulkanContext* VulkanContext::s_Context = nullptr;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData) {
        YZ_INFO("validation layer: " + std::string(pCallbackData->pMessage));
        return VK_FALSE;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
                                                                              "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
                                                                               "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    VulkanContext::VulkanContext(size_t width, size_t height) {
        init(width, height);
        s_Context = this;
    }

    VulkanContext::~VulkanContext() {
        m_ImageAvailableSemaphores.clear();
        m_RenderFinishedSemaphores.clear();

        m_Swapchain.reset();
        m_CommandPool.reset();

        Devices::release();

        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
        }
        if (m_Instance) {
            vkDestroyInstance(m_Instance, nullptr);
        }
    }

    void VulkanContext::init(size_t width, size_t height) {
        // Create our link between our APP and the vulkan library
        createInstance();
        // Setup some validation layers such that if theres an issue with us
        // Interacting with the library, then it may catch our faults.
        setupDebugMessenger();

        // Create our static device singleton
        m_Devices = Devices::instance();
        m_Devices->init(m_Instance);

        m_CommandPool = std::make_shared<CommandPool>();

        // Create a swapchain, a swapchain is responsible for maintaining the images
        // that will be presented to the user.
        m_Swapchain = std::make_shared<Swapchain>(width, height);

        m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    }

    void VulkanContext::onResize(size_t width, size_t height) {
        Devices::instance()->waitIdle();
        m_Swapchain->onResize(width, height);
    }

    bool VulkanContext::begin() {
        auto result = m_Swapchain->acquireNextImage(m_ImageAvailableSemaphores[m_CurrentFrame].getSemaphore());

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            return false;
        }
        else if (result != VK_SUCCESS) {
            YZ_ERROR("Vulkan failed to aquire a swapchain image.");
        }
        return true;
    }

    bool VulkanContext::present(CommandBuffer* cmdBuffer) {

        submitGfxQueue(cmdBuffer, true);

        VkResult result = m_Swapchain->present(m_RenderFinishedSemaphores[m_CurrentFrame].getSemaphore());

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            return false;
        }
        else if (result != VK_SUCCESS) {
            YZ_ERROR("Vulkan failed to present a swapchain image.");
        }

        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        return true;
    }

    void VulkanContext::submitGfxQueue(CommandBuffer* cmdBuffer, bool waitFence) {
        auto currentWaitSemaphore = m_ImageAvailableSemaphores[m_CurrentFrame].getSemaphore();
        auto currentSignalSemaphore = m_RenderFinishedSemaphores[m_CurrentFrame].getSemaphore();

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmdBuffer->getCommandBuffer();
        VkPipelineStageFlags flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submitInfo.pWaitDstStageMask = &flags;
        submitInfo.pWaitSemaphores = &currentWaitSemaphore;
        submitInfo.waitSemaphoreCount = (uint32_t)(currentWaitSemaphore ? 1 : 0);
        submitInfo.pSignalSemaphores = &currentSignalSemaphore;
        submitInfo.signalSemaphoreCount =  (uint32_t)(currentSignalSemaphore ? 1 : 0);
        submitInfo.pNext = VK_NULL_HANDLE;

        if (!waitFence) {
            vkQueueSubmit(m_Devices->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(m_Devices->getGraphicsQueue());
        }
        else {
            auto fence = cmdBuffer->getFence();
            vkQueueSubmit(m_Devices->getGraphicsQueue(), 1, &submitInfo, fence);
            vkWaitForFences(m_Devices->getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
            vkResetFences(m_Devices->getDevice(), 1, &fence);
        }
    }

    void VulkanContext::setupDebugMessenger() {
        // Only setup Validation layers if built in Debug Mode
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        auto res = CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger);
        if (res != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    void VulkanContext::createInstance() {
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            YZ_CRITICAL("VK Instance was unable to be created, no validation layers available");
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Yare Vulkan Renderer";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Yare";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_1;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS) {
            YZ_CRITICAL("VK Instance was unable to be created");
        }
    }

    void VulkanContext::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    std::vector<const char*> VulkanContext::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        extensions.push_back("VK_KHR_get_physical_device_properties2");

        return extensions;
    }

    bool VulkanContext::checkValidationLayerSupport() {
        uint32_t layerCount;
        bool layerFound = false;

        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) {
                return false;
            }
        }
        return layerFound;
    }

}
