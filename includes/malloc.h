/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 18:42:27 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/05 14:36:29 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <pthread.h>
#include <fcntl.h>
# include <unistd.h>
# include <sys/mman.h>
# include <stdlib.h>
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

// Zone sizes
/*
** For a pagesize of 4096 bytes
**
** TINY - block <= 128 bytes  - heap 16 KB	--> 4 pages
** SMALL - block <= 1024 bytes - heap 129 KB --> 32 pages
** LARGE - block > 1024 bytes
*/

# define TINY_BLOCK_MAX 128
# define SMALL_BLOCK_MAX 1024

# define LOGS_FILE "/tmp/malloc_debug.log"

// ========================
//       GLOBAL VARIABLES
// ========================

extern pthread_mutex_t	g_mutex;
extern t_heap			*g_heap;

// ========================
//       MAIN FUNCTIONS
// ========================

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem(void);
void	show_alloc_mem_ex(void);

// ========================
//      HELPER FUNCTIONS
// ========================

void	start_free(void *ptr);
void	*start_malloc(size_t size);
int		get_heap_size(size_t size);

t_heap_group	get_heap_type(size_t size);

void	find_block(size_t s, t_heap **res_heap, t_block **res_block);
t_block	*try_filling_block(size_t size);
t_heap	*create_heap(size_t size);
t_heap	*find_heap(size_t size);
t_block	*append_block(t_heap *heap, size_t size);

void	ptr_search(void *ptr, t_heap **ptr_heap, t_block **ptr_block);
t_block	*merge_block(t_heap *heap, t_block *block);
void	remove_block(t_heap *heap, t_block *block);
void	remove_heap(t_heap *heap);

void	print_group(t_heap *heap);
void	print_summary(size_t total_allocated, size_t total_blocks, size_t total_heaps);

size_t	heap_size(int pages);
size_t	align_size(size_t size);
void	*get_ptr(t_block *ptr);
t_block	*heap_shift(t_heap *ptr);
void	*end_of_ptr(t_block *ptr);

// ========================
//    ENVIRONMENT FLAGS
// ========================
typedef enum e_env
{
	ENV_DEBUG = (1 << 0),
	ENV_SHOW = (1 << 1),
	ENV_FILL = (1 << 2)
}	t_env;

// ========================
//    DEBUG FUNCTIONS
// ========================

int		getenv_cached(t_env env);
void	logs_show(void);
void	logs_debug(char *msg, size_t size);

// ========================
//      DEBUG FUNCTIONS
// ========================

void	show_heap(void);
void	print_block(t_block *block);
void	print_heap(t_heap *heap);

#endif
