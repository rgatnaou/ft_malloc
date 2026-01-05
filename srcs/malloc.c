#include "malloc.h"

// Simple malloc without zones (we'll add zones later)
void *malloc(size_t size)
{
    void *ptr;
    
    if (size == 0)
        return NULL;
    
    // Lock mutex for thread safety
    pthread_mutex_lock(&g_malloc_mutex);
    
    // Align the size
    size = align_size(size);
    
    // Simple mmap for now (we'll improve this later)
    ptr = allocate_memory(size + sizeof(t_block));
    
    if (ptr == NULL) {
        pthread_mutex_unlock(&g_malloc_mutex);
        return NULL;
    }
    
    // Setup block metadata
    t_block *block = (t_block *)ptr;
    block->size = size;
    block->free = 0;
    block->next = NULL;
    block->prev = NULL;
    block->data = (void *)((char *)ptr + sizeof(t_block));
    
    pthread_mutex_unlock(&g_malloc_mutex);
    
    // Return pointer to user data (after metadata)
    return block->data;
}