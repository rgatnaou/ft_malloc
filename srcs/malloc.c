/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 15:08:34 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/09 17:42:31 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "stdio.h"
// #include <sys/mman.h>
// #include <sys/resource.h>
// #include <unistd.h>
// #include <string.h>

// Simple malloc without zones (we'll add zones later)
void	*start_malloc(size_t size)
{
	t_heap	*heap;
	t_block	*block;

	if (!size)
		return (NULL);
	size = ALIGN(size);
	block = try_filling_block(size);
	if (block)
		return (BLOCK_SHIFT(block));
	heap = find_heap(size);
	if (!heap)
		return (NULL);
	append_block(heap, block, size);
	return (BLOCK_SHIFT(block));
}

void	*malloc(size_t size)
{
	void *block;

	pthread_mutex_lock(&g_mutex);
	// log_detail();
	block = start_malloc(size);
	if (block)
		ft_memset(block, 0, size);
	pthread_mutex_unlock(&g_mutex);
	return (block);
}


// int main ()
// {
// 	// struct rlimit limit;
// 	// int size = getpagesize();
// 	// void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
// 	// int rtn = getrlimit(RLIMIT_DATA, &limit);
// 	// char *str = "Hello, mmap!";
// 	// if (ptr == MAP_FAILED)
// 	// {
// 	// 	perror("mmap failed");
// 	// 	return 1;
// 	// }
// 	// else
// 	// {
// 	// 	ptr = memcpy(ptr, str, 13);
// 	// }
// 	// printf("size of page: %d\n", size);
// 	// printf("limit: %d: (%ld - %ld) \n", rtn, limit.rlim_cur, limit.rlim_max);
// 	// printf("ptr: %p: %x\n", ptr, (char *)ptr);
// 	// munmap(ptr, size);
// 	// printf("Memory unmapped successfully.\n");
// 	// printf("ptr after munmap: %p, %x:\n", ptr, (char *)ptr);
// 	char *ptr = malloc(42);
// 	ptr[0] = 'H';
// 	ptr[1] = 'i';
// 	printf("Allocated ptr: %p\n", ptr);
// 	printf("Content: %c%c\n", ptr[0], ptr[1]); 
// 	return 0;

// }
