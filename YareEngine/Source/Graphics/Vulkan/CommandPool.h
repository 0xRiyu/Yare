#ifndef YARE_COMMANDPOOL_H
#define YARE_COMMANDPOOL_H

#include "Graphics/Vulkan/Vk.h"

namespace Yare::Graphics {
    class YzVkCommandPool {
    public:
        YzVkCommandPool();
        ~YzVkCommandPool();

        void init();
        const VkCommandPool& getCommandPool() const { return m_CommandPool; }

    private:
        VkCommandPool m_CommandPool;
    };
}
#endif //YARE_COMMANDPOOL_H
