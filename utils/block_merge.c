/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_merge.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 16:44:33 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/10 18:51:05 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static t_block	*merge_prev_block(t_heap *heap, t_block *block)
{
	if (heap && block && block->prev && block->prev->is_free)
	{
		block->prev->next = block->next;
		if (block->next)
			block->next->prev = block->prev;
		block->prev->data_size += block->data_size + sizeof(t_block);
		heap->block_count--;
		return (block->prev);
	}
	return (block);
}

static void	merge_next_block(t_heap *heap, t_block *block)
{
	if (heap && block && block->next && block->next->is_free)
	{
		block->data_size += block->next->data_size + sizeof(t_block);
		if (block->next && block->next->next)
			block->next->next->prev = block;
		block->next = block->next->next;
		heap->block_count--;
	}
}

t_block	*merge_block(t_heap *heap, t_block *block)
{
	merge_next_block(heap, block);
	return (merge_prev_block(heap, block));
}
