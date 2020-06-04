#ifndef YAREZO_VK_SEMAPHORE_H
#define YAREZO_VK_SEMAPHORE_H

#include "Graphics/Vulkan/Vk.h"

namespace Yarezo::Graphics {

    class YzVkSemaphore {
    public:
        YzVkSemaphore();
        ~YzVkSemaphore();

        void init();

        const VkSemaphore& getSemaphore() const { return m_Semaphore; }

    private:
        VkSemaphore m_Semaphore = VK_NULL_HANDLE;
    };
}
#endif // YAREZO_VK_SEMAPHORE_H
