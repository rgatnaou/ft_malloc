#include "malloc.h"

void free(void *ptr)
{
    if (ptr == NULL)
        return;
    
    pthread_mutex_lock(&g_malloc_mutex);
    
    // Get block pointer from user pointer
    t_block *block = (t_block *)((char *)ptr - sizeof(t_block));
    
    // Mark as free
    block->free = 1;
    
    // TODO: Merge with adjacent free blocks
    
    // TODO: For LARGE blocks, munmap immediately
    
    pthread_mutex_unlock(&g_malloc_mutex);
}