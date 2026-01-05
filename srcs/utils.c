#include "malloc.h"

// Global variables (as declared in malloc.h)
t_zone *g_tiny_zones = NULL;
t_zone *g_small_zones = NULL;
t_zone *g_large_zones = NULL;
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

// Get system page size
size_t get_page_size(void)
{
    static size_t page_size = 0;
    
    if (page_size == 0) {
#ifdef __APPLE__
        page_size = getpagesize();
#else
        page_size = sysconf(_SC_PAGESIZE);
#endif
    }
    return page_size;
}

// Align size to 16 bytes
size_t align_size(size_t size)
{
    return ALIGN(size);
}

// Safe mmap wrapper
void *allocate_memory(size_t size)
{
    void *ptr;
    
    // Align size to page size
    size_t page_size = get_page_size();
    size_t aligned_size = ((size + page_size - 1) / page_size) * page_size;
    
    ptr = mmap(NULL, aligned_size,
               PROT_READ | PROT_WRITE,
               MAP_PRIVATE | MAP_ANONYMOUS,
               -1, 0);
    
    if (ptr == MAP_FAILED) {
        return NULL;
    }
    
    return ptr;
}

// Find zone type based on size
int get_zone_type(size_t size)
{
    if (size <= TINY_MAX)
        return 1;  // TINY
    else if (size <= SMALL_MAX)
        return 2;  // SMALL
    else
        return 3;  // LARGE
}

// Print hex address (for show_alloc_mem)
void print_hex_address(void *addr)
{
    // This will be used later
    (void)addr;
}