# ft_malloc

<div align="center">

**A Custom Dynamic Memory Allocator in C**

[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

*Reimplementing the C standard library's dynamic memory allocation functions using mmap*

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Architecture](#-architecture)
- [Memory Management Strategy](#-memory-management-strategy)
- [Function Reference](#-function-reference)
- [Building the Project](#-building-the-project)
- [Usage](#-usage)
- [Performance Considerations](#-performance-considerations)
- [Testing](#-testing)
- [Implementation Details](#-implementation-details)

---

## 🎯 Overview

**ft_malloc** is a custom implementation of the C standard library's dynamic memory allocation functions. This project recreates `malloc`, `free`, `realloc`, `calloc`, and `reallocf` from scratch using low-level system calls (`mmap` and `munmap`), providing deep insight into memory management at the operating system level.

This implementation is optimized for efficiency through a multi-zone allocation strategy, metadata management, and intelligent memory reuse, making it suitable as both an educational tool and a practical memory allocator.

### Why This Project?

- **Educational**: Understand how dynamic memory allocation works under the hood
- **Practical**: Learn about virtual memory, page alignment, and memory fragmentation
- **Performance**: Explore optimization techniques like memory pooling and zone-based allocation
- **Systems Programming**: Master low-level system calls and memory management

---

## ✨ Features

- ✅ **Complete malloc family**: `malloc`, `free`, `realloc`, `calloc`, `reallocf`
- 🎯 **Multi-zone allocation strategy**: Optimized for TINY, SMALL, and LARGE allocations
- 🔒 **Thread-safe operations**: Mutex protection for concurrent access
- 🔄 **Memory defragmentation**: Automatic coalescing of adjacent free blocks
- 📊 **Debug utilities**: Memory visualization and leak detection
- ⚡ **Optimized performance**: Minimizes system calls through intelligent zone management
- 🧩 **Shared library**: Can be used as a drop-in replacement for system malloc

---

## 🏗 Architecture

### Memory Organization

ft_malloc organizes memory into three distinct zones based on allocation size:

```
┌─────────────────────────────────────────────────────────────┐
│                      MEMORY ZONES                            │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐       │
│  │   TINY       │  │    SMALL     │  │    LARGE     │       │
│  │              │  │              │  │              │       │
│  │  ≤ 128 B     │  │  ≤ 1024 B    │  │  > 1024 B    │       │
│  │              │  │              │  │              │       │
│  │ [Zone 1]     │  │ [Zone 1]     │  │ [Allocation] │       │
│  │ [Zone 2]     │  │ [Zone 2]     │  │ [Allocation] │       │
│  │ [...]        │  │ [...]        │  │ [...]        │       │
│  └──────────────┘  └──────────────┘  └──────────────┘       │
│                                                               │
└─────────────────────────────────────────────────────────────┘
```

### Zone Structure

Each zone consists of:

```c
┌────────────────────────────────────────────┐
│              ZONE HEADER                   │
├────────────────────────────────────────────┤
│ - Zone type (TINY/SMALL/LARGE)             │
│ - Total size                                │
│ - Pointer to next zone                      │
│ - Number of free blocks                     │
└────────────────────────────────────────────┘
           ↓
┌────────────────────────────────────────────┐
│              BLOCK 1 METADATA              │
├────────────────────────────────────────────┤
│ - Size                                      │
│ - Free flag                                 │
│ - Pointer to next block                     │
└────────────────────────────────────────────┘
┌────────────────────────────────────────────┐
│              BLOCK 1 DATA                  │
│            (user memory)                    │
└────────────────────────────────────────────┘
           ↓
┌────────────────────────────────────────────┐
│              BLOCK 2 METADATA              │
└────────────────────────────────────────────┘
           ...
```

---

## 🧠 Memory Management Strategy

### Allocation Strategy

1. **Size Classification**:
   - **TINY**: Allocations ≤ 128 bytes
   - **SMALL**: Allocations ≤ 1024 bytes
   - **LARGE**: Allocations > 1024 bytes

2. **Zone Pre-allocation**:
   - TINY zones: Pre-allocated with space for 100 blocks
   - SMALL zones: Pre-allocated with space for 100 blocks
   - LARGE allocations: Individual mmap calls (no zone)

3. **First-Fit Algorithm**:
   - Search existing zones for a free block of sufficient size
   - If found, split the block if remainder is significant
   - If not found, allocate a new zone

### Deallocation Strategy

1. **Block Marking**: Mark the block as free
2. **Coalescing**: Merge with adjacent free blocks to reduce fragmentation
3. **Zone Cleanup**: If a zone becomes entirely free, unmap it (for LARGE only)

### Memory Alignment

All allocations are aligned to **16-byte boundaries** to ensure compatibility with CPU architecture requirements and improve cache performance.

---

## 📚 Function Reference

### `malloc()`

```c
void *malloc(size_t size);
```

**Purpose**: Allocates `size` bytes of memory and returns a pointer to the allocated memory.

**Algorithm**:
1. Determine allocation category (TINY/SMALL/LARGE)
2. Search for a suitable free block in existing zones
3. If no block found, allocate a new zone
4. Return aligned pointer to usable memory

**Returns**:
- Pointer to allocated memory on success
- `NULL` on failure or if `size` is 0

**Example**:
```c
int *numbers = malloc(10 * sizeof(int));
if (numbers == NULL) {
    // Handle allocation failure
}
```

---

### `free()`

```c
void free(void *ptr);
```

**Purpose**: Deallocates memory previously allocated by `malloc`, `calloc`, or `realloc`.

**Algorithm**:
1. Validate pointer (check if it was allocated by our allocator)
2. Retrieve block metadata
3. Mark block as free
4. Coalesce with adjacent free blocks
5. For LARGE allocations, unmap immediately
6. For TINY/SMALL, maintain zone for future allocations

**Behavior**:
- Does nothing if `ptr` is `NULL`
- Undefined behavior if `ptr` was not allocated by malloc or was already freed

**Example**:
```c
free(numbers);
numbers = NULL; // Good practice
```

---

### `realloc()`

```c
void *realloc(void *ptr, size_t size);
```

**Purpose**: Changes the size of the memory block pointed to by `ptr` to `size` bytes.

**Algorithm**:
1. If `ptr` is `NULL`, behave like `malloc(size)`
2. If `size` is 0, behave like `free(ptr)` and return `NULL`
3. Check if current block can accommodate new size:
   - If yes and larger: expand in place
   - If yes and smaller: shrink in place (possibly split block)
4. If expansion not possible:
   - Allocate new block of requested size
   - Copy existing data to new block
   - Free old block
   - Return pointer to new block

**Returns**:
- Pointer to reallocated memory (may be different from `ptr`)
- `NULL` on failure (original block remains valid)

**Example**:
```c
int *expanded = realloc(numbers, 20 * sizeof(int));
if (expanded == NULL) {
    // Original 'numbers' is still valid
    free(numbers);
} else {
    numbers = expanded;
}
```

---

### `calloc()`

```c
void *calloc(size_t count, size_t size);
```

**Purpose**: Allocates memory for an array of `count` elements of `size` bytes each and initializes all bytes to zero.

**Algorithm**:
1. Calculate total size: `total = count * size`
2. Check for overflow in multiplication
3. Call `malloc(total)`
4. Initialize all bytes to 0 using `memset` or equivalent
5. Return pointer to zeroed memory

**Returns**:
- Pointer to allocated and zeroed memory
- `NULL` on failure or overflow

**Example**:
```c
int *zeros = calloc(10, sizeof(int));
// All elements are guaranteed to be 0
```

---

### `reallocf()`

```c
void *reallocf(void *ptr, size_t size);
```

**Purpose**: BSD-specific variant of `realloc` that frees the original pointer on failure.

**Algorithm**:
1. Attempt reallocation using `realloc` logic
2. If reallocation fails:
   - Free the original pointer
   - Return `NULL`
3. If successful, return new pointer

**Returns**:
- Pointer to reallocated memory on success
- `NULL` on failure (original block is freed)

**Example**:
```c
numbers = reallocf(numbers, 20 * sizeof(int));
// If this fails, 'numbers' is already freed
if (numbers == NULL) {
    // Cannot use original pointer
}
```

---

## 🔨 Building the Project

### Prerequisites

- GCC or Clang compiler
- Make
- POSIX-compliant operating system (Linux, macOS, BSD)

### Compilation

```bash
# Clone the repository
git clone https://github.com/rgatnaou/ft_malloc.git
cd ft_malloc

# Build the shared library
make

# Build with debug symbols
make debug

# Clean build artifacts
make clean

# Complete clean (including library)
make fclean

# Rebuild everything
make re
```

### Build Outputs

- `libft_malloc_$(HOSTTYPE).so`: Platform-specific shared library
- `libft_malloc.so`: Symbolic link to the platform-specific library

---

## 💻 Usage

### Method 1: Direct Linking

```bash
# Compile your program with the library
gcc -o myprogram myprogram.c -L. -lft_malloc -Wl,-rpath,.

# Run your program
./myprogram
```

### Method 2: LD_PRELOAD (Runtime Replacement)

```bash
# Export library path
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

# Use LD_PRELOAD to replace system malloc
export DYLD_LIBRARY_PATH=.:$DYLD_LIBRARY_PATH  # macOS
export LD_PRELOAD=./libft_malloc.so            # Linux

# Run any program with your malloc
ls
```

### Method 3: Integration in C Projects

```c
// In your source file
#include <stdlib.h>  // Or your custom malloc.h

int main(void) {
    // Use malloc/free as normal
    char *str = malloc(100);
    if (str) {
        strcpy(str, "Hello from ft_malloc!");
        printf("%s\n", str);
        free(str);
    }
    
    // Use calloc
    int *array = calloc(10, sizeof(int));
    if (array) {
        free(array);
    }
    
    return 0;
}
```

### Debug Functions

```c
// Show memory allocation summary
void show_alloc_mem(void);

// Show detailed memory dump
void show_alloc_mem_ex(void);
```

Example output:
```
TINY : 0x100000000
0x100000028 - 0x100000128 : 256 bytes
0x100000228 - 0x100000428 : 512 bytes
SMALL : 0x100001000
0x100001028 - 0x100001428 : 1024 bytes
LARGE : 0x100010000
0x100010000 - 0x100020000 : 65536 bytes
Total : 67328 bytes
```

---

## ⚡ Performance Considerations

### Optimization Techniques

1. **Zone Pooling**: Pre-allocates memory zones to reduce system calls
2. **First-Fit**: Fast allocation by searching from zone start
3. **Coalescing**: Reduces fragmentation by merging free blocks
4. **Size Classes**: Separates allocations to reduce internal fragmentation
5. **Alignment**: 16-byte alignment improves CPU cache performance

### Time Complexity

| Operation | Best Case | Average Case | Worst Case |
|-----------|-----------|--------------|------------|
| malloc    | O(1)      | O(n)         | O(n)       |
| free      | O(1)      | O(n)         | O(n)       |
| realloc   | O(1)      | O(n + m)     | O(n + m)   |
| calloc    | O(m)      | O(n + m)     | O(n + m)   |

Where:
- `n` = number of blocks in a zone
- `m` = size of allocated memory (for zeroing/copying)

### Space Overhead

- **Metadata per block**: ~24-32 bytes (depending on architecture)
- **Zone header**: ~48 bytes per zone
- **Internal fragmentation**: Minimized through size classes
- **External fragmentation**: Reduced through coalescing

---

## 🧪 Testing

### Running Tests

```bash
# Run basic functionality tests
make test

# Run with different test programs
./run.sh test0  # Basic allocation/deallocation
./run.sh test1  # Stress test
./run.sh test2  # Thread safety test
./run.sh test3  # Fragmentation test
```

### Test Cases

The test suite includes:

1. **Basic Functionality**: Verify malloc, free, realloc work correctly
2. **Edge Cases**: Zero-size allocations, NULL pointers, double-free
3. **Stress Tests**: Thousands of allocations/deallocations
4. **Memory Leaks**: Verify all allocations are properly freed
5. **Thread Safety**: Concurrent allocations from multiple threads
6. **Performance**: Compare with system malloc

### Valgrind Testing

```bash
# Check for memory leaks
valgrind --leak-check=full ./your_program

# Use your allocator with valgrind
LD_PRELOAD=./libft_malloc.so valgrind --leak-check=full ls
```

---

## 🔍 Implementation Details

### Data Structures

```c
// Zone types
typedef enum {
    TINY,
    SMALL,
    LARGE
} t_zone_type;

// Block metadata
typedef struct s_block {
    size_t size;              // Size of usable memory
    int free;                 // Free flag
    struct s_block *next;     // Next block in zone
    struct s_block *prev;     // Previous block (for coalescing)
    char data[1];             // Flexible array member (start of user data)
} t_block;

// Zone header
typedef struct s_zone {
    t_zone_type type;         // TINY, SMALL, or LARGE
    size_t size;              // Total zone size
    size_t free_blocks;       // Number of free blocks
    struct s_zone *next;      // Next zone in linked list
    t_block *blocks;          // First block in zone
} t_zone;

// Global state
typedef struct s_malloc_state {
    t_zone *tiny;             // TINY zones list
    t_zone *small;            // SMALL zones list
    t_zone *large;            // LARGE allocations list
    pthread_mutex_t lock;     // Thread safety
} t_malloc_state;
```

### Key Algorithms

#### Block Splitting

When a free block is larger than needed:

```c
void split_block(t_block *block, size_t size) {
    // Calculate split point
    size_t original_size = block->size;
    if (original_size - size > sizeof(t_block) + MINIMUM_SPLIT_SIZE) {
        // Create new block from remainder
        t_block *new_block = (t_block *)((char *)block + sizeof(t_block) + size);
        new_block->size = original_size - size - sizeof(t_block);
        new_block->free = 1;
        new_block->next = block->next;
        new_block->prev = block;
        
        // Update original block
        block->size = size;
        block->next = new_block;
        
        // Update next block's prev pointer
        if (new_block->next) {
            new_block->next->prev = new_block;
        }
    }
}
```

#### Block Coalescing

Merge adjacent free blocks:

```c
void coalesce_blocks(t_block *block) {
    // Merge with next block if free
    if (block->next && block->next->free) {
        block->size += sizeof(t_block) + block->next->size;
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }
    
    // Merge with previous block if free
    if (block->prev && block->prev->free) {
        block->prev->size += sizeof(t_block) + block->size;
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
    }
}
```

### Thread Safety

All public functions are protected by a mutex:

```c
void *malloc(size_t size) {
    pthread_mutex_lock(&g_malloc_state.lock);
    void *ptr = internal_malloc(size);
    pthread_mutex_unlock(&g_malloc_state.lock);
    return ptr;
}
```

---

## 🎓 Learning Resources

### Understanding Memory Allocation

- [Memory Allocation Tutorial](https://www.memorymanagement.org/mmref/index.html#mmref-intro)
- [Virtual Memory and Heaps](https://www.cprogramming.com/tutorial/virtual_memory_and_heaps.html)
- [mmap vs brk](https://cboard.cprogramming.com/linux-programming/101090-what-differences-between-brk-mmap.html)

### System Calls

- [mmap(2) man page](https://linux.die.net/man/2/mmap)
- [munmap(2) man page](https://linux.die.net/man/2/munmap)
- [Memory-mapped I/O](http://www.gnu.org/software/libc/manual/html_node/Memory_002dmapped-I_002fO.html)

### Standards

- [malloc(3) man page](https://linux.die.net/man/3/malloc)
- [C Standard Library Documentation](http://www.gnu.org/software/libc/manual/)

---

## 🙏 Acknowledgments

- **42 School** for the project specifications and learning framework
- **The GNU C Library** for inspiration and reference implementation
- **Operating Systems: Three Easy Pieces** for memory management concepts
- All contributors and testers who helped improve this project

---

## 📬 Contact

**Author**: rgatnaou

**Repository**: [https://github.com/rgatnaou/ft_malloc](https://github.com/rgatnaou/ft_malloc)

---

<div align="center">

**⭐ If you found this project helpful, please consider giving it a star! ⭐**

Made with ❤️ and lots of ☕

</div>