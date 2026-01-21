/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_append.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:21:08 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/21 18:35:43 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	fill_block(t_block *block, size_t size)
{
	block->data_size = size;
	block->is_free = 0;
	block->next = NULL;
	block->prev = NULL;
}

t_block	*append_block(t_heap *heap, size_t size)
{
	t_block	*last_block;
	t_block	*new_block;

	if (!heap)
		return (NULL);
	last_block = (t_block *)HEAP_SHIFT(heap);
	if (heap->block_count == 0)
	{
		new_block = (t_block *)HEAP_SHIFT(heap);
		fill_block(new_block, size);
		heap->free_size -= (size + sizeof(t_block));
	}
	else
	{
		while (last_block->next)
			last_block = last_block->next;
		new_block = (t_block *)((char *)BLOCK_SHIFT(last_block)
				+ last_block->data_size);
		fill_block(new_block, size);
		new_block->prev = last_block;
		last_block->next = new_block;
		heap->free_size -= (size + sizeof(t_block));
	}
	heap->block_count++;
	return (new_block);
}
