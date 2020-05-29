#include "Graphics/Vulkan/Vk_Image.h"
#include "Graphics/Vulkan/Vk_Devices.h"
#include "Graphics/Vulkan/Vk_Utilities.h"
#include "Utilities/YzLogger.h"

#include <stb/stb_image.h>
#include <stdlib.h>

namespace Yarezo::Graphics {

    YzVkImage::YzVkImage() {
    }

    YzVkImage::~YzVkImage() {
        if (m_ImageView) {
            vkDestroyImageView(Graphics::YzVkDevice::instance()->getDevice(), m_ImageView, nullptr);
        }
        if (m_Image) {
            vkDestroyImage(Graphics::YzVkDevice::instance()->getDevice(), m_Image, nullptr);
        }
        if (m_ImageMemory) {
            vkFreeMemory(Graphics::YzVkDevice::instance()->getDevice(), m_ImageMemory, nullptr);
        }
        if (m_Sampler) {
            vkDestroySampler(Graphics::YzVkDevice::instance()->getDevice(), m_Sampler, nullptr);
        }
    }

    void YzVkImage::createTexture2DFromFile(const std::string& filePath) {
        YzVkBuffer stagingBuffer;
        loadTextureFromFileIntoBuffer(filePath, stagingBuffer);
        createTexture2D(stagingBuffer);
    }

    void YzVkImage::createTextureCubeFromFile(const std::string& filePath) {
        YzVkBuffer stagingBuffer;
        loadTextureFromFileIntoBuffer(filePath, stagingBuffer);
        createTextureCube(stagingBuffer);
    }

    void YzVkImage::createTextureCubeFromFiles(const std::vector<std::string>& filePaths) {
        YzVkBuffer stagingBuffer;
        loadTexturesFromFilesIntoBuffer(filePaths, stagingBuffer);
        createTextureCube(stagingBuffer);
    }

    void YzVkImage::createEmptyTexture(size_t width, size_t height, VkFormat format,
                                       VkImageTiling tiling, VkImageUsageFlags usage,
                                       VkMemoryPropertyFlags properties, VkImageAspectFlagBits flagBits) {
        m_TextureWidth = width;
        m_TextureHeight = height;
        createImage(VK_IMAGE_TYPE_2D, format, tiling, usage, 0, properties);
        m_ImageView = VkUtil::createImageView(m_Image, VK_IMAGE_VIEW_TYPE_2D, format, 1, flagBits);
    }

    void YzVkImage::loadTextureFromFileIntoBuffer(const std::string& filePath, YzVkBuffer& buffer) {

        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(filePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        m_TextureWidth = static_cast<size_t>(texWidth);
        m_TextureHeight = static_cast<size_t>(texHeight);

        if (!pixels) {
            YZ_CRITICAL("stbi_load failed to load a texture from file.");
        }

        buffer.init(VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    imageSize, pixels);

        stbi_image_free(pixels);
    }

    void YzVkImage::loadTexturesFromFilesIntoBuffer(const std::vector<std::string>& filePaths,
                                                    YzVkBuffer& buffer) {
        // allocate some memory for us to store our images in sequence
        // width * height * r/g/b/a * 20 images
        uint32_t max_size = 2048 * 2048 * 4 * 20;
        unsigned char* images = (unsigned char*)malloc(max_size);
        size_t total_images_size = 0;

        for (const auto& filePath : filePaths) {
            // Load each image into a buffer
            int texWidth, texHeight, texChannels;
            stbi_uc* image = stbi_load(filePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
            if (!image) {
                YZ_CRITICAL("stbi_load failed to load a texture from file.");
            }
            VkDeviceSize imageSize = texWidth * texHeight * 4 /* STBI_rgb_alpha */;
            if (total_images_size + imageSize > max_size) {
                YZ_CRITICAL("VK_Image: Not enough memory was allocated to store images");
            }
            // Copy the image into our buffer so we can access it later based on an offset
            memcpy(images, image, imageSize);
            // Increment the ptr to the starting address of the next image we want to store
            images += imageSize;

            // Keep track of how many bytes were copied into the buffer
            total_images_size += imageSize;

            // I dont know how to handle textures of different sizes yet
            m_TextureWidth = static_cast<size_t>(texWidth);
            m_TextureHeight = static_cast<size_t>(texHeight);

            stbi_image_free(image);
        }

        images -= total_images_size;

        buffer.init(VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    total_images_size, images);
    }

    void YzVkImage::createTexture2D(const YzVkBuffer& buffer) {
        createImage(VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB,
                    VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                    0,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        m_ImageView = VkUtil::createImageView(m_Image, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB,
                                              1, VK_IMAGE_ASPECT_COLOR_BIT);

        transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, 1, VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        copyBufferToImage(buffer, 1, 1);
        transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, 1, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        createSampler(VK_SAMPLER_ADDRESS_MODE_REPEAT);
    }

    void YzVkImage::createTextureCube(const YzVkBuffer& buffer) {
        createImage(VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB,
                    VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                    VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        m_ImageView = VkUtil::createImageView(m_Image, VK_IMAGE_VIEW_TYPE_CUBE, VK_FORMAT_R8G8B8A8_SRGB,
                                              6, VK_IMAGE_ASPECT_COLOR_BIT);

        transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, 6, VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        copyBufferToImage(buffer, 6, 1);

        transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, 6, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        createSampler(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
    }

    void YzVkImage::transitionImageLayout(VkFormat format, uint32_t layerCount, VkImageLayout oldLayout, VkImageLayout newLayout) {
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
        barrier.subresourceRange.layerCount = layerCount;

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
            throw std::invalid_argument("unsupported layout transition!");
        }

        VkCommandBuffer commandBuffer = VkUtil::beginSingleTimeCommands();

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

    void YzVkImage::copyBufferToImage(const YzVkBuffer& buffer, uint32_t faces, uint32_t mipLevels) {

        std::vector<VkBufferImageCopy> bufferCopyRegions;

        for (uint32_t face = 0; face < faces; face++) {
            for (uint32_t level = 0; level < mipLevels; level++) {
                // This assumes the buffer has only n required images and nothing more
                uint32_t offset = (static_cast<uint32_t>(buffer.getSize()) / faces) * face;
                VkBufferImageCopy bufferCopyRegion = {};
                bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                bufferCopyRegion.imageSubresource.mipLevel = level;
                bufferCopyRegion.imageSubresource.baseArrayLayer = face;
                bufferCopyRegion.imageSubresource.layerCount = 1;
                bufferCopyRegion.imageExtent = {
                                      static_cast<uint32_t>(m_TextureWidth),
                                      static_cast<uint32_t>(m_TextureHeight),
                                      1
                };
                bufferCopyRegion.bufferOffset = offset;
                bufferCopyRegions.push_back(bufferCopyRegion);
            }
        }

        VkCommandBuffer commandBuffer = VkUtil::beginSingleTimeCommands();
        vkCmdCopyBufferToImage(commandBuffer, buffer.getBuffer(), m_Image,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               static_cast<uint32_t>(bufferCopyRegions.size()), bufferCopyRegions.data());
        VkUtil::endSingleTimeCommands(commandBuffer);
    }

    void YzVkImage::createImage(VkImageType type, VkFormat format, VkImageTiling tiling,
                                VkImageUsageFlags usage, VkImageCreateFlags flags,
                                VkMemoryPropertyFlags properties) {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = type;
        imageInfo.extent.width = static_cast<uint32_t>(m_TextureWidth);
        imageInfo.extent.height = static_cast<uint32_t>(m_TextureHeight);
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.flags = flags;

        if (flags == VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) {
            imageInfo.arrayLayers = 6;
        } else {
            imageInfo.arrayLayers = 1;
        }

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

    void YzVkImage::createSampler(VkSamplerAddressMode mode) {
        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = mode;
        samplerInfo.addressModeV = mode;
        samplerInfo.addressModeW = mode;
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

    YzVkImage* YzVkImage::createDepthStencilBuffer(size_t width, size_t height, VkFormat format) {
        YzVkImage* image = new YzVkImage();
        image->createEmptyTexture(width, height, format, VK_IMAGE_TILING_OPTIMAL,
                                  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);
        return image;
    }

    YzVkImage* YzVkImage::createTexture2D(const std::string& filePath) {
        YzVkImage* image = new YzVkImage();
        image->createTexture2DFromFile(filePath);
        return image;
    }

    YzVkImage* YzVkImage::createTextureCube(const std::vector<std::string>& filePaths) {
        YzVkImage* image = new YzVkImage();
        if (filePaths.empty()) {
            YZ_ERROR("Texture Cube filepaths vector is empty.");
        }
        if (filePaths.size() == 1) {
            image->createTextureCubeFromFile(filePaths[0]);
        } else {
            image->createTextureCubeFromFiles(filePaths);
        }
        return image;
    }
}
