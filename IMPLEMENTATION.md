<# Building Your Own malloc: A Complete Technical Implementation Guide

*A step-by-step walkthrough of implementing custom memory allocation from first principles*

---

## Table of Contents

1. [Why Build Your Own malloc?](#why-build-your-own-malloc)
2. [Understanding the Foundation](#understanding-the-foundation)
3. [Setting Up the Data Structures](#setting-up-the-data-structures)
4. [Implementing malloc()](#implementing-malloc)
5. [Implementing free()](#implementing-free)
6. [Implementing realloc()](#implementing-realloc)
7. [Implementing calloc()](#implementing-calloc)
8. [Implementing reallocf()](#implementing-reallocf)
9. [Advanced Optimizations](#advanced-optimizations)
10. [Testing Your Implementation](#testing-your-implementation)

---

## Why Build Your Own malloc?

Before we dive into the code, let's understand what we're building and why it matters.

Every time you call `malloc(100)` in C, you're asking for 100 bytes of memory. But where does that memory come from? The answer involves a fascinating dance between your program, the `malloc` library, and the operating system kernel.

The key insight is this: **system calls are expensive**. If we called `mmap` (the OS syscall) for every tiny allocation, our programs would crawl. That's why `malloc` exists as a middleman—it requests large chunks from the OS and then manages them efficiently for your program.

This project will teach you:
- How virtual memory actually works
- Why fragmentation is a real problem and how to fight it
- Performance trade-offs in memory management
- Low-level systems programming techniques

Let's build it from scratch. 🚀

---

## Understanding the Foundation

### The System Calls We Need

We'll be using two primary system calls:

```c
#include <sys/mman.h>

// Request memory from the OS
void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);

// Return memory to the OS
int munmap(void *addr, size_t len);
```

Here's what the `mmap` parameters mean:

- `addr`: Suggested address (we'll use `NULL` to let the OS decide)
- `len`: Number of bytes to map
- `prot`: Memory protection (`PROT_READ | PROT_WRITE` for read/write access)
- `flags`: `MAP_PRIVATE | MAP_ANON` for anonymous private mapping
- `fd`: File descriptor (-1 for anonymous mapping)
- `offset`: File offset (0 for anonymous mapping)

### Why Not Just Use mmap Directly?

If you called `mmap` for every allocation, even tiny ones, you'd face two major problems:

1. **Performance**: Context switching to kernel mode is expensive (microseconds per call)
2. **Granularity**: `mmap` works with whole memory pages (typically 4096 bytes). Allocating 10 bytes would waste 4086 bytes!

Our `malloc` will solve this by:
- Requesting large chunks (heaps) from the OS
- Subdividing them into smaller blocks
- Tracking which blocks are free
- Reusing freed blocks

### Memory Page Size: A Critical Optimization

Get your system's page size:

```bash
getconf PAGE_SIZE  # Usually 4096 bytes
```

**Pro tip**: Always allocate memory in multiples of the page size. This aligns with how the OS manages memory and prevents waste.

---

## Setting Up the Data Structures

### The Two-Level Hierarchy

We'll organize memory into two levels:

```
┌─────────────────────────────────────────────────┐
│                   HEAP (mmap'd region)          │
│  ┌────────────────────────────────────────┐    │
│  │ Heap Header (metadata)                  │    │
│  ├────────────────────────────────────────┤    │
│  │ Block 1 Header                          │    │
│  ├────────────────────────────────────────┤    │
│  │ Block 1 Data (user memory)              │    │
│  ├────────────────────────────────────────┤    │
│  │ Block 2 Header                          │    │
│  ├────────────────────────────────────────┤    │
│  │ Block 2 Data                            │    │
│  └────────────────────────────────────────┘    │
└─────────────────────────────────────────────────┘
```

### Heap Structure

```c
typedef enum {
    TINY,   // Small allocations (≤ 128 bytes)
    SMALL,  // Medium allocations (≤ 1024 bytes)
    LARGE   // Big allocations (> 1024 bytes)
} t_heap_group;

typedef struct s_heap {
    struct s_heap   *prev;          // Previous heap in linked list
    struct s_heap   *next;          // Next heap in linked list
    t_heap_group    group;          // TINY, SMALL, or LARGE
    size_t          total_size;     // Total size of this heap
    size_t          free_size;      // Remaining free space
    size_t          block_count;    // Number of blocks in this heap
} t_heap;
```

**Why a linked list?** As our program runs, we'll create multiple heaps. Linking them lets us search through all available memory to find free blocks.

### Block Structure

```c
typedef struct s_block {
    struct s_block  *prev;      // Previous block (for coalescing)
    struct s_block  *next;      // Next block (for traversal)
    size_t          data_size;  // Size of user data
    bool            freed;      // Is this block available?
} t_block;
```

### Helper Macros

These macros help us navigate between headers and data:

```c
// Jump from heap header to the first block
#define HEAP_SHIFT(start)   ((void *)start + sizeof(t_heap))

// Jump from block header to user data
#define BLOCK_SHIFT(start)  ((void *)start + sizeof(t_block))

// Get block header from user pointer
#define GET_BLOCK(ptr)      ((t_block *)((void *)ptr - sizeof(t_block)))

// Get heap header from block
#define GET_HEAP(block)     ((t_heap *)((void *)block - block_offset))
```

### Global State

```c
typedef struct s_malloc_state {
    t_heap          *tiny;      // Linked list of TINY heaps
    t_heap          *small;     // Linked list of SMALL heaps
    t_heap          *large;     // Linked list of LARGE allocations
    pthread_mutex_t lock;       // Thread safety
} t_malloc_state;

// Global instance
static t_malloc_state g_state = {NULL, NULL, NULL, PTHREAD_MUTEX_INITIALIZER};
```

---

## Implementing malloc()

### The High-Level Algorithm

```c
void *malloc(size_t size) {
    pthread_mutex_lock(&g_state.lock);
    
    // 1. Align size to 16 bytes (CRITICAL for compatibility)
    size = ALIGN(size);
    
    // 2. Determine heap group
    t_heap_group group = get_group(size);
    
    // 3. Find or create a suitable heap
    t_heap *heap = find_heap(group, size);
    if (!heap) {
        heap = create_heap(group, size);
        if (!heap) {
            pthread_mutex_unlock(&g_state.lock);
            return NULL;
        }
    }
    
    // 4. Find or create a block within the heap
    t_block *block = find_block(heap, size);
    if (!block) {
        block = create_block(heap, size);
    }
    
    pthread_mutex_unlock(&g_state.lock);
    return BLOCK_SHIFT(block);  // Return pointer to user data
}
```

### Step 1: Size Alignment

**This is critical!** Many CPU architectures and programs expect pointers to be aligned to 16-byte boundaries.

```c
#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
```

**How this works:**
- `(size + 15)` rounds up to the next alignment boundary
- `& ~15` zeroes out the last 4 bits, ensuring multiple of 16

Example:
```c
ALIGN(10)  = 16   // 10 + 15 = 25 → 25 & ~15 = 16
ALIGN(16)  = 16   // Already aligned
ALIGN(17)  = 32   // 17 + 15 = 32 → 32 & ~15 = 32
```

**Why alignment matters:** Without this, programs like `vim` will crash with segmentation faults. I learned this the hard way!

### Step 2: Determine Heap Group

```c
#define TINY_BLOCK_SIZE  128
#define SMALL_BLOCK_SIZE 1024

t_heap_group get_group(size_t size) {
    if (size <= TINY_BLOCK_SIZE)
        return TINY;
    else if (size <= SMALL_BLOCK_SIZE)
        return SMALL;
    else
        return LARGE;
}
```

**Strategy:**
- TINY and SMALL get pre-allocated heaps (memory pools)
- LARGE allocations get individual `mmap` calls

### Step 3: Calculate Heap Sizes

We want to fit at least 100 blocks per heap to amortize the `mmap` cost:

```c
#define getpagesize() 4096  // Standard page size

// For TINY: 100 blocks of 128 bytes each
#define TINY_HEAP_ALLOCATION_SIZE  (4 * getpagesize())   // 16,384 bytes
#define TINY_BLOCKS_PER_HEAP       (TINY_HEAP_ALLOCATION_SIZE / TINY_BLOCK_SIZE)

// For SMALL: 100 blocks of 1024 bytes each  
#define SMALL_HEAP_ALLOCATION_SIZE (16 * getpagesize())  // 65,536 bytes
#define SMALL_BLOCKS_PER_HEAP      (SMALL_HEAP_ALLOCATION_SIZE / SMALL_BLOCK_SIZE)
```

**Why multiples of page size?** The OS allocates memory in pages anyway. If we request 5000 bytes, we get 8192 (2 pages). Might as well use the full 8192!

### Step 4: Find an Existing Heap

```c
t_heap *find_heap(t_heap_group group, size_t size) {
    t_heap *heap = get_heap_list(group);
    
    while (heap) {
        // Check if this heap has enough free space
        if (heap->free_size >= size + sizeof(t_block)) {
            return heap;
        }
        heap = heap->next;
    }
    
    return NULL;  // No suitable heap found
}

t_heap *get_heap_list(t_heap_group group) {
    switch (group) {
        case TINY:  return g_state.tiny;
        case SMALL: return g_state.small;
        case LARGE: return g_state.large;
    }
}
```

### Step 5: Create a New Heap

```c
t_heap *create_heap(t_heap_group group, size_t size) {
    size_t heap_size;
    
    if (group == TINY) {
        heap_size = TINY_HEAP_ALLOCATION_SIZE;
    } else if (group == SMALL) {
        heap_size = SMALL_HEAP_ALLOCATION_SIZE;
    } else {
        // LARGE: allocate exactly what's needed
        heap_size = sizeof(t_heap) + sizeof(t_block) + size;
        heap_size = ALIGN_PAGE(heap_size);
    }
    
    // Request memory from OS
    void *ptr = mmap(NULL, heap_size, 
                     PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANON,
                     -1, 0);
    
    if (ptr == MAP_FAILED) {
        return NULL;
    }
    
    // Initialize heap header
    t_heap *heap = (t_heap *)ptr;
    heap->prev = NULL;
    heap->next = NULL;
    heap->group = group;
    heap->total_size = heap_size;
    heap->free_size = heap_size - sizeof(t_heap);
    heap->block_count = 0;
    
    // Add to appropriate linked list
    add_heap_to_list(heap, group);
    
    return heap;
}

void add_heap_to_list(t_heap *heap, t_heap_group group) {
    t_heap **list_head;
    
    switch (group) {
        case TINY:  list_head = &g_state.tiny; break;
        case SMALL: list_head = &g_state.small; break;
        case LARGE: list_head = &g_state.large; break;
    }
    
    if (*list_head) {
        (*list_head)->prev = heap;
    }
    heap->next = *list_head;
    *list_head = heap;
}
```

### Step 6: Find or Create a Block

```c
t_block *find_block(t_heap *heap, size_t size) {
    // Start from the first block position
    t_block *block = (t_block *)HEAP_SHIFT(heap);
    
    for (size_t i = 0; i < heap->block_count; i++) {
        if (block->freed && block->data_size >= size) {
            // Found a free block that's big enough!
            block->freed = false;
            return block;
        }
        
        // Move to next block
        block = (t_block *)((void *)block + sizeof(t_block) + block->data_size);
    }
    
    return NULL;  // No suitable free block
}

t_block *create_block(t_heap *heap, size_t size) {
    // Calculate where the new block should go
    t_block *block = (t_block *)HEAP_SHIFT(heap);
    
    // Skip to the end of existing blocks
    for (size_t i = 0; i < heap->block_count; i++) {
        block = (t_block *)((void *)block + sizeof(t_block) + block->data_size);
    }
    
    // Initialize block header
    block->prev = (heap->block_count > 0) ? get_previous_block(block) : NULL;
    block->next = NULL;
    block->data_size = size;
    block->freed = false;
    
    // Update heap metadata
    heap->block_count++;
    heap->free_size -= (sizeof(t_block) + size);
    
    // Link previous block to this one
    if (block->prev) {
        block->prev->next = block;
    }
    
    return block;
}
```

### Complete malloc() Implementation

```c
void *malloc(size_t size) {
    if (size == 0) {
        return NULL;  // Undefined behavior, we choose to return NULL
    }
    
    pthread_mutex_lock(&g_state.lock);
    
    // Align size
    size = ALIGN(size);
    
    // Determine heap group
    t_heap_group group = get_group(size);
    
    // Find existing heap with space
    t_heap *heap = find_heap(group, size);
    
    // Or create new heap
    if (!heap) {
        heap = create_heap(group, size);
        if (!heap) {
            pthread_mutex_unlock(&g_state.lock);
            return NULL;
        }
    }
    
    // Try to find a free block
    t_block *block = find_block(heap, size);
    
    // Or create a new block
    if (!block) {
        if (heap->free_size < sizeof(t_block) + size) {
            // Not enough space in this heap either
            pthread_mutex_unlock(&g_state.lock);
            return malloc(size);  // Recursively try again (will create new heap)
        }
        block = create_block(heap, size);
    }
    
    pthread_mutex_unlock(&g_state.lock);
    
    // Return pointer to user data (skip the header)
    return BLOCK_SHIFT(block);
}
```

---

## Implementing free()

### The High-Level Algorithm

```c
void free(void *ptr) {
    if (!ptr) {
        return;  // Freeing NULL is a no-op
    }
    
    pthread_mutex_lock(&g_state.lock);
    
    // 1. Get block header from user pointer
    t_block *block = GET_BLOCK(ptr);
    
    // 2. Mark block as freed
    block->freed = true;
    
    // 3. Coalesce with adjacent free blocks
    coalesce_blocks(block);
    
    // 4. If heap is empty, return it to OS
    t_heap *heap = find_heap_for_block(block);
    if (should_free_heap(heap)) {
        remove_heap(heap);
        munmap(heap, heap->total_size);
    }
    
    pthread_mutex_unlock(&g_state.lock);
}
```

### The Fragmentation Problem

When we simply mark blocks as freed, we create a problem:

```
Before free():
┌────────┬────────┬────────┬────────┐
│ Block1 │ Block2 │ Block3 │ Block4 │
│  (50)  │  (30)  │  (40)  │  (60)  │
└────────┴────────┴────────┴────────┘

After freeing blocks 2 and 3:
┌────────┬────────┬────────┬────────┐
│ Block1 │ XXXXXX │ XXXXXX │ Block4 │
│  (50)  │  (30)  │  (40)  │  (60)  │
└────────┴────────┴────────┴────────┘

Problem: We have 70 bytes free, but can't satisfy a 70-byte allocation!
```

**Solution: Coalescing** — Merge adjacent free blocks into one large block.

### Implementing Coalescing

```c
void coalesce_blocks(t_block *block) {
    t_block *current = block;
    
    // Merge with next blocks
    while (current->next && current->next->freed) {
        t_block *next = current->next;
        
        // Absorb the next block
        current->data_size += sizeof(t_block) + next->data_size;
        current->next = next->next;
        
        if (next->next) {
            next->next->prev = current;
        }
        
        // Update heap metadata
        t_heap *heap = find_heap_for_block(current);
        heap->block_count--;
        heap->free_size += sizeof(t_block);
    }
    
    // Merge with previous blocks
    while (current->prev && current->prev->freed) {
        t_block *prev = current->prev;
        
        // Absorb current into previous
        prev->data_size += sizeof(t_block) + current->data_size;
        prev->next = current->next;
        
        if (current->next) {
            current->next->prev = prev;
        }
        
        // Update heap metadata
        t_heap *heap = find_heap_for_block(prev);
        heap->block_count--;
        heap->free_size += sizeof(t_block);
        
        current = prev;
    }
}
```

### Deciding When to Free a Heap

```c
bool should_free_heap(t_heap *heap) {
    if (heap->group == LARGE) {
        // LARGE allocations are one block per heap
        // If the block is freed, the whole heap is empty
        t_block *block = (t_block *)HEAP_SHIFT(heap);
        return block->freed;
    }
    
    // For TINY/SMALL, only free if:
    // 1. All blocks are freed
    // 2. We have at least one other heap of this type (keep one warm)
    if (heap->free_size == heap->total_size - sizeof(t_heap)) {
        t_heap *list = get_heap_list(heap->group);
        int count = 0;
        t_heap *temp = list;
        while (temp) {
            count++;
            temp = temp->next;
        }
        return count > 1;  // Only free if we have backups
    }
    
    return false;
}

void remove_heap(t_heap *heap) {
    // Update linked list
    if (heap->prev) {
        heap->prev->next = heap->next;
    } else {
        // This was the head of the list
        t_heap **list_head;
        switch (heap->group) {
            case TINY:  list_head = &g_state.tiny; break;
            case SMALL: list_head = &g_state.small; break;
            case LARGE: list_head = &g_state.large; break;
        }
        *list_head = heap->next;
    }
    
    if (heap->next) {
        heap->next->prev = heap->prev;
    }
}
```

### Complete free() Implementation

```c
void free(void *ptr) {
    if (!ptr) {
        return;
    }
    
    pthread_mutex_lock(&g_state.lock);
    
    // Get block header
    t_block *block = GET_BLOCK(ptr);
    
    // Validate block (detect double-free)
    if (block->freed) {
        pthread_mutex_unlock(&g_state.lock);
        return;  // Or handle error
    }
    
    // Mark as free
    block->freed = true;
    
    // Coalesce with neighbors
    coalesce_blocks(block);
    
    // Check if we should return heap to OS
    t_heap *heap = find_heap_for_block(block);
    if (should_free_heap(heap)) {
        remove_heap(heap);
        munmap(heap, heap->total_size);
    }
    
    pthread_mutex_unlock(&g_state.lock);
}
```

---

## Implementing realloc()

### The Strategy

`realloc` is conceptually simple but has many edge cases:

```c
void *realloc(void *ptr, size_t size) {
    // Case 1: realloc(NULL, size) == malloc(size)
    if (!ptr) {
        return malloc(size);
    }
    
    // Case 2: realloc(ptr, 0) == free(ptr) [we return ptr instead]
    if (size == 0) {
        return ptr;  // Lazy approach, avoids edge cases
    }
    
    pthread_mutex_lock(&g_state.lock);
    
    t_block *block = GET_BLOCK(ptr);
    size_t old_size = block->data_size;
    
    // Align new size
    size = ALIGN(size);
    
    // Case 3: New size fits in current block
    if (size <= old_size) {
        // Could split the block if difference is significant
        if (old_size - size > sizeof(t_block) + ALIGNMENT) {
            split_block(block, size);
        }
        pthread_mutex_unlock(&g_state.lock);
        return ptr;
    }
    
    // Case 4: Try to expand in place
    if (can_expand_in_place(block, size)) {
        expand_block(block, size);
        pthread_mutex_unlock(&g_state.lock);
        return ptr;
    }
    
    pthread_mutex_unlock(&g_state.lock);
    
    // Case 5: Allocate new block, copy data, free old block
    void *new_ptr = malloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, old_size);
        free(ptr);
    }
    
    return new_ptr;
}
```

### Expanding in Place

```c
bool can_expand_in_place(t_block *block, size_t new_size) {
    size_t needed = new_size - block->data_size;
    
    // Check if next block exists and is free
    if (!block->next || !block->next->freed) {
        return false;
    }
    
    // Check if next block is big enough
    size_t available = block->next->data_size + sizeof(t_block);
    return available >= needed;
}

void expand_block(t_block *block, size_t new_size) {
    size_t needed = new_size - block->data_size;
    t_block *next = block->next;
    
    // Absorb the next block
    block->data_size += sizeof(t_block) + next->data_size;
    block->next = next->next;
    
    if (next->next) {
        next->next->prev = block;
    }
    
    // Update heap
    t_heap *heap = find_heap_for_block(block);
    heap->block_count--;
    heap->free_size += sizeof(t_block);
    
    // If we absorbed more than needed, split
    if (block->data_size > new_size + sizeof(t_block) + ALIGNMENT) {
        split_block(block, new_size);
    }
}
```

### Splitting Blocks

```c
void split_block(t_block *block, size_t size) {
    size_t remaining = block->data_size - size;
    
    // Only split if remainder is worth it
    if (remaining <= sizeof(t_block) + ALIGNMENT) {
        return;
    }
    
    // Create new block from the remainder
    t_block *new_block = (t_block *)((void *)block + sizeof(t_block) + size);
    new_block->data_size = remaining - sizeof(t_block);
    new_block->freed = true;
    new_block->prev = block;
    new_block->next = block->next;
    
    // Update original block
    block->data_size = size;
    block->next = new_block;
    
    // Update next block's prev pointer
    if (new_block->next) {
        new_block->next->prev = new_block;
    }
    
    // Update heap
    t_heap *heap = find_heap_for_block(block);
    heap->block_count++;
}
```

---

## Implementing calloc()

`calloc` is malloc + zeroing:

```c
void *calloc(size_t count, size_t size) {
    // Check for overflow
    if (count != 0 && size > SIZE_MAX / count) {
        return NULL;  // Multiplication would overflow
    }
    
    size_t total = count * size;
    
    void *ptr = malloc(total);
    if (ptr) {
        memset(ptr, 0, total);  // Zero out the memory
    }
    
    return ptr;
}
```

**Note on overflow:** This check is critical! Without it:
```c
calloc(0x100000000, 8)  // count * size wraps to 0
→ malloc(0)             // Allocates nothing
→ memset crashes        // Writing to NULL or invalid memory
```

---

## Implementing reallocf()

This is a BSD extension that frees the original pointer on failure:

```c
void *reallocf(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    
    if (!new_ptr && ptr && size != 0) {
        // Realloc failed, free the original
        free(ptr);
    }
    
    return new_ptr;
}
```

**Use case:** Simplifies error handling:

```c
// Without reallocf:
new_ptr = realloc(ptr, new_size);
if (!new_ptr) {
    free(ptr);  // Must remember to free
    return error;
}
ptr = new_ptr;

// With reallocf:
ptr = reallocf(ptr, new_size);
if (!ptr) {
    return error;  // Already freed
}
```

---

## Advanced Optimizations

### 1. Thread Safety

Always protect shared state:

```c
static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

void *malloc(size_t size) {
    pthread_mutex_lock(&g_lock);
    void *ptr = internal_malloc(size);
    pthread_mutex_unlock(&g_lock);
    return ptr;
}
```

### 2. Debugging Support

Add compile-time debug features:

```c
#ifdef DEBUG_MALLOC
void show_alloc_mem(void) {
    pthread_mutex_lock(&g_lock);
    
    printf("TINY : %p\n", g_state.tiny);
    print_heap_details(g_state.tiny);
    
    printf("SMALL : %p\n", g_state.small);
    print_heap_details(g_state.small);
    
    printf("LARGE : %p\n", g_state.large);
    print_heap_details(g_state.large);
    
    pthread_mutex_unlock(&g_lock);
}

void print_heap_details(t_heap *heap) {
    while (heap) {
        t_block *block = (t_block *)HEAP_SHIFT(heap);
        for (size_t i = 0; i < heap->block_count; i++) {
            if (!block->freed) {
                printf("%p - %p : %zu bytes\n",
                       BLOCK_SHIFT(block),
                       (void *)BLOCK_SHIFT(block) + block->data_size,
                       block->data_size);
            }
            block = (t_block *)((void *)block + sizeof(t_block) + block->data_size);
        }
        heap = heap->next;
    }
}
#endif
```

### 3. Memory Poisoning (Detect Use-After-Free)

```c
#ifdef DEBUG_MALLOC
void free(void *ptr) {
    // ... normal free logic ...
    
    // Poison freed memory with pattern
    memset(ptr, 0xDEADBEEF, block->data_size);
    
    // ... rest of free ...
}
#endif
```

### 4. Canary Values (Detect Buffer Overflow)

```c
#define CANARY 0xDEADC0DE

typedef struct s_block {
    uint32_t canary_start;
    // ... normal fields ...
    size_t data_size;
    bool freed;
    uint32_t canary_end;
} t_block;

void validate_block(t_block *block) {
    if (block->canary_start != CANARY || block->canary_end != CANARY) {
        fprintf(stderr, "CORRUPTION DETECTED at %p\n", block);
        abort();
    }
}
```

---

## Testing Your Implementation

### Basic Functionality Test

```c
int main(void) {
    // Test 1: Basic malloc/free
    char *str = malloc(100);
    assert(str != NULL);
    strcpy(str, "Hello, malloc!");
    printf("%s\n", str);
    free(str);
    
    // Test 2: Multiple allocations
    int *arr1 = malloc(10 * sizeof(int));
    int *arr2 = malloc(20 * sizeof(int));
    int *arr3 = malloc(15 * sizeof(int));
    assert(arr1 && arr2 && arr3);
    free(arr1);
    free(arr2);
    free(arr3);
    
    // Test 3: Realloc
    int *nums = malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++) nums[i] = i;
    nums = realloc(nums, 10 * sizeof(int));
    assert(nums[0] == 0 && nums[4] == 4);
    free(nums);
    
    // Test 4: Calloc
    int *zeros = calloc(10, sizeof(int));
    for (int i = 0; i < 10; i++) {
        assert(zeros[i] == 0);
    }
    free(zeros);
    
    printf("All tests passed!\n");
    return 0;
}
```

### Stress Test

```c
void stress_test(void) {
    void *ptrs[1000];
    
    // Allocate many blocks
    for (int i = 0; i < 1000; i++) {
        size_t size = (rand() % 2000) + 1;
        ptrs[i] = malloc(size);
        assert(ptrs[i] != NULL);
    }
    
    // Free in random order
    for (int i = 999; i >= 0; i--) {
        int idx = rand() % (i + 1);
        free(ptrs[idx]);
        ptrs[idx] = ptrs[i];
    }
    
    printf("Stress test passed!\n");
}
```

### Testing with Real Programs

Create a script to inject your malloc:

```bash
#!/bin/bash
# run.sh

export DYLD_LIBRARY_PATH=.               # macOS
export LD_LIBRARY_PATH=.                 # Linux
export DYLD_INSERT_LIBRARIES=libft_malloc.so
export DYLD_FORCE_FLAT_NAMESPACE=1
export LD_PRELOAD=./libft_malloc.so

$@
```

Usage:
```bash
chmod +x run.sh
./run.sh ls -la
./run.sh python3 script.py
./run.sh vim file.txt
```

### Valgrind Testing

```bash
# Check for memory leaks
valgrind --leak-check=full ./your_program

# Use your malloc with valgrind
LD_PRELOAD=./libft_malloc.so valgrind --leak-check=full ls
```

---

## Common Pitfalls and Solutions

### 1. The Alignment Bug

**Problem:** Programs crash with SIGBUS or segfault.

**Solution:** Always align sizes:
```c
size = (size + 15) & ~15;
```

### 2. Double-Free Detection

**Problem:** Freeing the same pointer twice corrupts the heap.

**Solution:** Check the freed flag:
```c
void free(void *ptr) {
    t_block *block = GET_BLOCK(ptr);
    if (block->freed) {
        return;  // Already freed
    }
    block->freed = true;
    // ...
}
```

### 3. Fragmentation

**Problem:** Many small free blocks can't satisfy large allocations.

**Solution:** Implement coalescing (merge adjacent free blocks).

### 4. Memory Leaks in Error Paths

**Problem:** Forgetting to `munmap` when allocation fails.

**Solution:** Always clean up on error:
```c
t_heap *create_heap(size_t size) {
    void *ptr = mmap(...);
    if (ptr == MAP_FAILED) {
        return NULL;
    }
    
    t_heap *heap = (t_heap *)ptr;
    // ... initialization ...
    
    if (some_error) {
        munmap(ptr, size);
        return NULL;
    }
    
    return heap;
}
```

---

## Performance Benchmarks

Here's how to measure your implementation:

```c
#include <time.h>

void benchmark_malloc(void) {
    clock_t start, end;
    
    start = clock();
    for (int i = 0; i < 1000000; i++) {
        void *ptr = malloc(100);
        free(ptr);
    }
    end = clock();
    
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("1M malloc/free: %.3f seconds\n", cpu_time);
}
```

Compare with system malloc:
```bash
gcc -o test_system test.c
gcc -o test_custom test.c -L. -lft_malloc

time ./test_system
time ./test_custom
```

---

## Conclusion

You've now built a complete memory allocator from scratch! Here's what we covered:

1. ✅ Understanding why `malloc` exists (performance vs. system calls)
2. ✅ Designing a two-level heap/block hierarchy
3. ✅ Implementing `malloc()` with zone-based allocation
4. ✅ Implementing `free()` with coalescing
5. ✅ Implementing `realloc()` with in-place expansion
6. ✅ Implementing `calloc()` and `reallocf()`
7. ✅ Adding thread safety and debugging features
8. ✅ Testing with real-world programs

### Next Steps

- **Profile your allocator**: Use `perf` or `Instruments` to find bottlenecks
- **Optimize coalescing**: Try best-fit instead of first-fit
- **Add memory pooling**: Pre-allocate common sizes for speed
- **Implement tcmalloc strategies**: Thread-local caches to reduce lock contention
- **Write comprehensive tests**: Edge cases, concurrency, stress tests

The complete implementation is available at: [https://github.com/rgatnaou/ft_malloc](https://github.com/rgatnaou/ft_malloc)

Happy hacking! 🚀

---

## Additional Resources

- [Understanding glibc malloc](https://sourceware.org/glibc/wiki/MallocInternals)
- [TCMalloc Design Doc](https://google.github.io/tcmalloc/design.html)
- [jemalloc Technical Documentation](http://jemalloc.net/)
- [Memory Allocation Guide](https://www.kernel.org/doc/html/latest/core-api/memory-allocation.html)
- [Virtual Memory Concepts](https://www.kernel.org/doc/gorman/html/understand/understand.html)

---

*This guide is based on the article "Master memory management, I built my own malloc" by Jean-Baptiste Terrazzoni and adapted for the ft_malloc project implementation.*
