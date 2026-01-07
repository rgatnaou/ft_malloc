/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 15:08:34 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/07 21:03:01 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "stdio.h"
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>

// Simple malloc without zones (we'll add zones later)
void	*start_malloc(size_t size)
{
	t_heap	*heap;
	t_block	*block;
	void	*res;

	if (!size)
		return (NULL);
	size = (size + 15) & ~15;
	if ((block = try_filling_available_block(size)))
		return (BLOCK_SHIFT(block));

	return (res);
}

void	*malloc(size_t size)
{
	void *res;
	pthread_mutex_lock(&g_mutex);
	// log_detail();
	if ((res = start_malloc(size)) && getenv_cached(ENV_SCRIBBLE))
	return (res);
}


int main ()
{
	struct rlimit limit;
	int size = getpagesize();
	void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	int rtn = getrlimit(RLIMIT_DATA, &limit);
	char *str = "Hello, mmap!";
	if (ptr == MAP_FAILED)
	{
		perror("mmap failed");
		return 1;
	}
	else
	{
		ptr = memcpy(ptr, str, 13);
	}
	printf("size of page: %d\n", size);
	printf("limit: %d: (%ld - %ld) \n", rtn, limit.rlim_cur, limit.rlim_max);
	printf("ptr: %p: %x\n", ptr, (char *)ptr);
	munmap(ptr, size);
	printf("Memory unmapped successfully.\n");
	printf("ptr after munmap: %p, %x:\n", ptr, (char *)ptr);
	return 0;

}
