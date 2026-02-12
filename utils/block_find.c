/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_find.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 20:22:09 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/12 18:00:00 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static int	search_in_heap(t_heap *heap, size_t s, t_block **res)
{
	t_block	*block;

	block = heap_shift(heap);
	while (block)
	{
		if ((char *)block < (char *)heap + sizeof(t_heap)
			|| (char *)block >= (char *)heap + heap->total_size)
			break ;
		if (block->is_free && block->data_size >= s)
		{
			*res = block;
			return (1);
		}
		block = block->next;
	}
	return (0);
}

void	find_block(size_t s, t_heap **res_heap, t_block **res_block)
{
	t_heap			*heap;
	t_heap_group	group;

	group = get_heap_type(s);
	heap = g_heap;
	while (heap)
	{
		if (heap->type == group && heap->block_count > 0)
		{
			if (search_in_heap(heap, s, res_block))
			{
				*res_heap = heap;
				return ;
			}
		}
		heap = heap->next;
	}
	*res_block = NULL;
	*res_heap = NULL;
}
