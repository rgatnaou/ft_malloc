/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 18:42:27 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/10 18:48:15 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <stdio.h>
# include <pthread.h>
#include <unistd.h>
# include <sys/mman.h>
// # include <sys/resource.h>
# include "../libft/includes/libft.h"

// This is how we'll ask the OS for memory. sys/mman.h provides these.
// void	*mmap(void *addr, size_t len, int prot,int flags, int fd, off_t offset);
// int	munmap(void *addr, size_t len);

// And these help us set some ground rules. sys/resource.h provides these.
// int	getrlimit(int resource, struct rlimit* rlp);
// int	setrlimit(int resource, const struct rlimit* rlp);

// Data structures and constants for our malloc implementation.

// ========================
//        ENUMERATIONS
// ========================
typedef enum e_heap_type
{
	TINY,
	SMALL,
	LARGE
}	t_heap_group;


// ========================
//       DATA STRUCTURES
// ========================

typedef struct s_heap
{
	struct s_heap	*prev;
	struct s_heap	*next;
	t_heap_group	type;
	size_t			total_size;
	size_t			free_size;
	size_t			block_count;
}	t_heap;

typedef struct s_block
{
	struct s_block	*prev;
	struct s_block	*next;
	size_t			data_size;
	int				is_free;
}	t_block;

// ========================
//       CONSTANTS
// ========================

// Zone sizes (must be multiple of page size)
# define TINY_HEAP_SIZE (4 * getpagesize())   // N
# define SMALL_HEAP_SIZE (16 * getpagesize())  // M

// n: maximum size for tiny allocations
# define TINY_BLOCK_MAX (TINY_HEAP_SIZE / 128)
// n + 1: minimum size for small allocations
# define TINY_BLOCK_MIN 16

// m: maximum size for small allocations
# define SMALL_BLOCK_MAX (SMALL_HEAP_SIZE / 128)
// m + 1: minimum size for large allocations
# define SMALL_BLOCK_MIN (TINY_BLOCK_MAX + 1)



// Memory alignment (usually 16 bytes for x86_64)
# define ALIGNMENT 16
# define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

// Shift to get user data from heap metadata
# define HEAP_SHIFT(heap) ((void *)((char *)(heap) + sizeof(t_heap)))
// Shift to get user data from block metadata
# define BLOCK_SHIFT(block) ((void *)((char *)(block) + sizeof(t_block)))



// ========================
//       GLOBAL VARIABLES
// ========================
extern pthread_mutex_t		g_mutex;
extern t_heap				*g_heap;

// ========================
//       MAIN FUNCTIONS
// ========================
void	*ft_malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);

// ========================
//      HELPER FUNCTIONS
// ========================

int				get_heap_size(size_t size);
t_heap_group	get_heap_type(size_t size);
void			find_block(size_t s, t_heap **res_heap,t_block **res_block);
t_block			*try_filling_block(size_t size);
t_heap			*create_heap(size_t size);
t_heap			*find_heap(size_t size);
t_block			*append_block(t_heap *heap, size_t size);

void	ptr_search(void *ptr, t_heap **ptr_heap, t_block **ptr_block);
t_block	*merge_block(t_heap *heap, t_block *block);
void	remove_block(t_heap *heap, t_block *block);
void	remove_heap(t_heap *heap);

// ========================
//      DEBUG FUNCTIONS
// ========================

void			show_heap(void);
void			print_block(t_block *block);
void			print_heap(t_heap *heap);

#endif
