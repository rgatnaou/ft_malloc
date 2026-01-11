/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 20:22:21 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/11 16:34:57 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	split_block(t_heap *heap, t_block *block, size_t size)
{
	t_block	*new_block;

	new_block = (t_block *)((char *)BLOCK_SHIFT(block) + size);
	new_block->data_size = block->data_size - size - sizeof(t_block);
	new_block->is_free = 1;
	new_block->prev = block;
	new_block->next = block->next;
	if (block->next)
		block->next->prev = new_block;
	block->next = new_block;
	block->data_size = size;
	heap->block_count++;
	heap->free_size -= sizeof(t_block);
}

t_block	*try_filling_block(size_t size)
{
	t_heap				*heap;
	t_block				*block;

	find_block(size, &heap, &block);
	if (!block)
		return (NULL);
	split_block(heap, block, size);
	block->is_free = 0;
	return (block);
}
