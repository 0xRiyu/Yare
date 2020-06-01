#ifndef YAREZO_MEMORY_H
#define YAREZO_MEMORY_H

namespace Yarezo {
    // Wrapper functions for aligned memory allocation
    // There is currently no standard for this in C++ that works across all platforms and vendors, so we abstract this
    void* alignedAlloc(size_t size, size_t alignment);

    void alignedFree(void* data);
}

#endif //YAREZO_MEMORY_H
