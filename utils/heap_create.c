/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:21:47 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/21 18:04:23 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

// static getlimit_t	get_system_limit(void)
// {
// 	struct rlimit	rpl;

// 	if (getrlimit(RLIMIT_DATA, &rpl) < 0)
// 		return (-1);
// 	return (rpl.rlim_max);
// }

t_heap	*create_heap(size_t size)
{
	t_heap	*heap;
	size_t	heap_size;

	heap_size = get_heap_size(size);
	// if (heap_size > get_system_limit())
	// 	return (NULL);
	heap = mmap(NULL, heap_size, PROT_READ | PROT_WRITE,
			MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (heap == MAP_FAILED)
		return (NULL);
	heap->type = get_heap_type(size);
	heap->total_size = heap_size;
	heap->free_size = heap_size - sizeof(t_heap);
	if (g_heap)
	{
		heap->next = g_heap;
		heap->next->prev = heap;
	}
	else
		heap->next = NULL;
	heap->prev = NULL;
	g_heap = heap;
	heap->block_count = 0;
	return (heap);
}
