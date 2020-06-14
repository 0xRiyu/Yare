#ifndef YARE_MEMORY_H
#define YARE_MEMORY_H

namespace Yare {
    // Wrapper functions for aligned memory allocation
    // There is currently no standard for this in C++ that works across all platforms and vendors, so we abstract this
    void* alignedAlloc(size_t size, size_t alignment);

    void alignedFree(void* data);
}

#endif //YARE_MEMORY_H
