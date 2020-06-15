#ifndef YARE_COMMANDPOOL_H
#define YARE_COMMANDPOOL_H

#include "Graphics/Vulkan/Vk.h"

namespace Yare::Graphics {
    class CommandPool {
    public:
        CommandPool();
        ~CommandPool();

        void init();
        const VkCommandPool& getPool() const { return m_CommandPool; }

    private:
        VkCommandPool m_CommandPool;
    };
}
#endif //YARE_COMMANDPOOL_H
