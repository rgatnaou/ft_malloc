/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_remove.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 18:27:59 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/10 19:41:03 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	remove_heap(t_heap *heap)
{
	if (heap->block_count == 0)
	{
		if (heap->prev)
			heap->prev->next = heap->next;
		if (heap->next)
			heap->next->prev = heap->prev;
		if (g_heap == heap)
			g_heap = heap->next;
		munmap(heap, heap->total_size);
	}
}
