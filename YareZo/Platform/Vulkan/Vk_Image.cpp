#include "Platform/Vulkan/Vk_Image.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Utilities/YzLogger.h"
#include "Platform/Vulkan/Vk_Utilities.h"
#include "Platform/Vulkan/Vk_Buffer.h"

#include <stb_image.h>

namespace Yarezo {
	namespace Graphics {

        ///////////////////////////////////////////////////////////////////////////////////////////
        // YzVkSampler::
        ///////////////////////////////////////////////////////////////////////////////////////////

		YzVkSampler::YzVkSampler() {
		}
		YzVkSampler::~YzVkSampler() {
		}

		void YzVkSampler::init() {
            VkSamplerCreateInfo samplerInfo = {};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;
            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.anisotropyEnable = VK_TRUE;
            samplerInfo.maxAnisotropy = 16;
            samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_FALSE;
            samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerInfo.mipLodBias = 0.0f;
            samplerInfo.minLod = 0.0f;
            samplerInfo.maxLod = 0.0f;

            if (vkCreateSampler(Graphics::YzVkDevice::instance()->getDevice(), &samplerInfo, nullptr, &m_Sampler) != VK_SUCCESS) {
                YZ_CRITICAL("Vulkan failed to create a sampler.");
            }
		}

		void YzVkSampler::cleanUp() {
            if (m_Sampler != VK_NULL_HANDLE) {
                vkDestroySampler(Graphics::YzVkDevice::instance()->getDevice(), m_Sampler, nullptr);
            }
		}

        ///////////////////////////////////////////////////////////////////////////////////////////
        // YzVkImage::
        ///////////////////////////////////////////////////////////////////////////////////////////

        YzVkImage::YzVkImage() {
        }
        YzVkImage::~YzVkImage() {
        }

        void YzVkImage::init(uint32_t width, uint32_t height, VkBuffer buffer) {
            createImage(width, height, VK_FORMAT_R8G8B8A8_SRGB,
                VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            createImageView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);


            transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            VkUtil::copyBufferToImage(buffer, m_Image, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
            transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

            createSampler();
        }


        void YzVkImage::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties) {
            VkImageCreateInfo imageInfo = {};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = width;
            imageInfo.extent.height = height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = 1;
            imageInfo.format = format;
            imageInfo.tiling = tiling;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = usage;
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateImage(Graphics::YzVkDevice::instance()->getDevice(), &imageInfo, nullptr, &m_Image) != VK_SUCCESS) {
                YZ_CRITICAL("Failed to create an image.");
            }

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(Graphics::YzVkDevice::instance()->getDevice(), m_Image, &memRequirements);

            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = Graphics::VkUtil::findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            if (vkAllocateMemory(Graphics::YzVkDevice::instance()->getDevice(), &allocInfo, nullptr, &m_ImageMemory) != VK_SUCCESS) {
                YZ_CRITICAL("failed to allocate image memory!");
            }

            if (vkBindImageMemory(Graphics::YzVkDevice::instance()->getDevice(), m_Image, m_ImageMemory, 0) != VK_SUCCESS) {
                YZ_ERROR("Failed to bind image memory");
            }
        }

        void YzVkImage::createImageView(VkFormat format, VkImageAspectFlags flags) {
            //m_ImageView = VkUtil::createImageView(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
            m_ImageView = VkUtil::createImageView(m_Image, format, flags);
        }

        void YzVkImage::createSampler() {
            m_YzSampler.init();
        }

        void YzVkImage::transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
            VkCommandBuffer commandBuffer = Graphics::VkUtil::beginSingleTimeCommands();

            VkImageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = oldLayout;
            barrier.newLayout = newLayout;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = m_Image;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

            VkPipelineStageFlags sourceStage;
            VkPipelineStageFlags destinationStage;

            if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

                sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            }
            else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            }
            else {
                YZ_CRITICAL("unsupported layout transition!");
            }

            vkCmdPipelineBarrier(
                commandBuffer,
                sourceStage, destinationStage,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
            );

            VkUtil::endSingleTimeCommands(commandBuffer);
        }


        void YzVkImage::cleanUp() {
            if (m_ImageView) {
                vkDestroyImageView(Graphics::YzVkDevice::instance()->getDevice(), m_ImageView, nullptr);
            }
            if (m_Image) {
                vkDestroyImage(Graphics::YzVkDevice::instance()->getDevice(), m_Image, nullptr);
            }
            if (m_ImageMemory) {
                vkFreeMemory(Graphics::YzVkDevice::instance()->getDevice(), m_ImageMemory, nullptr);
            }
            m_YzSampler.cleanUp();
        }


        YzVkImage* YzVkImage::createEmptyTexture(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlagBits flagBits) {
            YzVkImage* image = new YzVkImage();
            image->createImage(width, height, format, tiling, usage, properties);
            image->createImageView(format, flagBits);
            return image;
        }

        YzVkImage* YzVkImage::createDepthStencilBuffer(VkFormat format, uint32_t width, uint32_t height) {
            return createEmptyTexture(width, height, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);
        }

        YzVkImage* YzVkImage::createTexture2D(const std::string& filePath) {
            YzVkImage* image = new YzVkImage();


            int texWidth, texHeight, texChannels;
            stbi_uc* pixels = stbi_load(filePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
            VkDeviceSize imageSize = texWidth * texHeight * 4;

            if (!pixels) {
                YZ_CRITICAL("failed to load texture image");
            }

            YzVkBuffer stagingBuffer {VK_BUFFER_USAGE_TRANSFER_SRC_BIT, imageSize, pixels};

            stbi_image_free(pixels);

            image->init(texWidth, texHeight, stagingBuffer.getBuffer());

            stagingBuffer.cleanUp();

            return image;
        }
    }
}