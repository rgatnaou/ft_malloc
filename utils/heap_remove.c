/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_remove.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 18:27:59 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/04 21:32:06 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

int	last_heap(t_heap *heap)
{
	int				count;
	t_heap_group	type;

	type = heap->type;
	count = 0;
	while (heap)
	{
		if (heap->type == type)
			count++;
		heap = heap->next;
	}
	return (count == 1);
}

void	remove_heap(t_heap *heap)
{
	if (heap->block_count == 0)
	{
		if (heap->type != LARGE && last_heap(heap))
			return ;
		if (heap->prev)
			heap->prev->next = heap->next;
		if (heap->next)
			heap->next->prev = heap->prev;
		if (g_heap == heap)
		{
			g_heap = heap->next;
			if (g_heap)
				g_heap->prev = NULL;
		}
		munmap(heap, heap->total_size);
	}
}
