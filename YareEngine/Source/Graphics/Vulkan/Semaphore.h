#ifndef YARE_SEMAPHORE_H
#define YARE_SEMAPHORE_H

#include "Graphics/Vulkan/Vk.h"

namespace Yare::Graphics {

    class Semaphore {
    public:
        Semaphore();
        ~Semaphore();

        void init();

        const VkSemaphore& getSemaphore() const { return m_Semaphore; }

    private:
        VkSemaphore m_Semaphore = VK_NULL_HANDLE;
    };
}
#endif // YARE_SEMAPHORE_H
