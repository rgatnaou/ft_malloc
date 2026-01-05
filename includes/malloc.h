#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <sys/mman.h>
# include <pthread.h>

// ========================
//       CONSTANTS
// ========================
# define TINY_MAX   128     // n: maximum size for tiny allocations
# define SMALL_MAX  1024    // m: maximum size for small allocations

// Zone sizes (must be multiple of page size)
# define TINY_ZONE_SIZE   (4 * getpagesize())   // N
# define SMALL_ZONE_SIZE  (16 * getpagesize())  // M

// Memory alignment (usually 16 bytes for x86_64)
# define ALIGNMENT 16
# define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

// ========================
//       DATA STRUCTURES
// ========================

// Metadata for each memory block
typedef struct s_block {
    size_t          size;      // Size of the user data
    int             free;      // 1 if free, 0 if allocated
    struct s_block *next;      // Next block in the same zone
    struct s_block *prev;      // Previous block
    void           *data;      // Pointer to user data (after metadata)
} t_block;

// Memory zone (contains multiple blocks)
typedef struct s_zone {
    size_t          total_size;    // Total size of the zone
    t_block        *first_block;   // First block in this zone
    struct s_zone  *next;          // Next zone of same type
    struct s_zone  *prev;          // Previous zone
} t_zone;

// ========================
//       GLOBAL VARIABLES
// ========================
extern t_zone *g_tiny_zones;
extern t_zone *g_small_zones;
extern t_zone *g_large_zones;
extern pthread_mutex_t g_malloc_mutex;

// ========================
//       MAIN FUNCTIONS
// ========================
void    *malloc(size_t size);
void    free(void *ptr);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem(void);

// ========================
//      HELPER FUNCTIONS
// ========================
size_t  get_page_size(void);
void    *allocate_memory(size_t size);
t_zone  *create_zone(size_t zone_size, size_t block_size);
t_block *find_free_block(t_zone *zone, size_t size);
t_block *create_block(void *memory, size_t size);
void    split_block(t_block *block, size_t size);
void    merge_free_blocks(t_zone *zone);

#endif