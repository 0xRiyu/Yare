
#include "Vk_Swapchain.h"
#include "Utilities/YzLogger.h"
#include "src/Application.h"
#include "Platform/Vulkan/Vk_Devices.h"

namespace Yarezo {
    namespace Graphics {
        YzVkSwapchain::YzVkSwapchain() {
        }
        YzVkSwapchain::~YzVkSwapchain() {
        }

        void YzVkSwapchain::init() {
            // Create a swapchain, a swapchain is responsible for maintaining the images
            // that will be presented to the user. 
            createSwapchain();
            // Create and image-view, which will represent a 'view' of an image,
            // this way we can interface with images without modifying the underlying image
            createImageViews();
        }

        void YzVkSwapchain::cleanUp() {

            for (auto& imageView : m_SwapchainImageViews) {
                vkDestroyImageView(YzVkDevice::instance()->getDevice(), imageView, nullptr);
            }

            if (m_Swapchain) {
                vkDestroySwapchainKHR(YzVkDevice::instance()->getDevice(), m_Swapchain, nullptr);
            }
        }

        void YzVkSwapchain::createSwapchain() {
            auto YzVkDeviceinstance = YzVkDevice::instance();

            SwapChainSupportDetails swapChainSupport = YzVkDeviceinstance->getSwapChainSupport();

            auto surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
            auto presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
            auto extent = chooseSwapExtent(swapChainSupport.capabilities);

            // Request one more image to render to than the minimum so we dont wait for driver operations
            uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

            if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
                imageCount = swapChainSupport.capabilities.maxImageCount;
            }

            VkSwapchainCreateInfoKHR createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = YzVkDeviceinstance->getSurfaceKHR();
            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            Graphics::QueueFamilyIndices indices = YzVkDeviceinstance->getQueueFamilyIndicies();
            uint32_t queueFamilyIndices[] = { static_cast<uint32_t>(indices.graphicsFamily), static_cast<uint32_t>(indices.presentFamily) };

            if (indices.graphicsFamily != indices.presentFamily) {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            }
            else {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            }

            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;
            createInfo.oldSwapchain = VK_NULL_HANDLE;

            if (vkCreateSwapchainKHR(YzVkDeviceinstance->getDevice(), &createInfo, nullptr, &m_Swapchain) != VK_SUCCESS) {
                YZ_ERROR("SwapChain failed to create");
                throw std::runtime_error("SwapChain failed to create.");
            }

            vkGetSwapchainImagesKHR(YzVkDeviceinstance->getDevice(), m_Swapchain, &imageCount, nullptr);
            m_SwapchainImages.resize(imageCount);
            vkGetSwapchainImagesKHR(YzVkDeviceinstance->getDevice(), m_Swapchain, &imageCount, m_SwapchainImages.data());

            m_SwapchainImageFormat = surfaceFormat.format;
            m_SwapchainExtent = extent;
        }

        void YzVkSwapchain::createImageViews() {
            size_t swapchainImagesSize = getImagesSize();

            m_SwapchainImageViews.resize(swapchainImagesSize);

            for (uint32_t i = 0; i < swapchainImagesSize; i++) {
                VkImageViewCreateInfo createInfo = {};
                createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                createInfo.image = getImage(i);
                createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                createInfo.format = getImageFormat();

                createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

                createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                createInfo.subresourceRange.baseMipLevel = 0;
                createInfo.subresourceRange.levelCount = 1;
                createInfo.subresourceRange.baseArrayLayer = 0;
                createInfo.subresourceRange.layerCount = 1;

                if (vkCreateImageView(YzVkDevice::instance()->getDevice(), &createInfo, nullptr, &m_SwapchainImageViews[i]) != VK_SUCCESS) {
                    YZ_ERROR("Failed to create image views.");
                    throw std::runtime_error("Failed to create image views.");
                }
            }
        }

        VkSurfaceFormatKHR YzVkSwapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
            for (const auto& availableFormat : availableFormats) {
                if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
                    availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    return availableFormat;
                }
            }
            return availableFormats[0];
        }

        VkPresentModeKHR YzVkSwapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
            for (const auto& availablePresentMode : availablePresentModes) {
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    return availablePresentMode;
                }
            }
            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D YzVkSwapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
            if (capabilities.currentExtent.width != UINT32_MAX) {
                return capabilities.currentExtent;
            }
            else {
                int width, height;
                glfwGetFramebufferSize(static_cast<GLFWwindow*>(Application::getAppInstance()->getWindow()->getNativeWindow()), &width, &height);

                Application::getAppInstance()->getWindow()->getCamera()->updateDimensions((float)width, (float)height);

                VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

                actualExtent.width = std::max(capabilities.minImageExtent.width,
                    std::min(capabilities.maxImageExtent.width, actualExtent.width));
                actualExtent.height = std::max(capabilities.minImageExtent.height,
                    std::min(capabilities.maxImageExtent.height, actualExtent.height));

                return actualExtent;
            }
        }
    }
}