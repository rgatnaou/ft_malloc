/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_append.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:21:08 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/10 19:01:43 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_block	*append_block(t_heap *heap, size_t size)
{
	t_block	*last_block;
	t_block	*new_block;

	if (!heap)
		return (NULL);
	last_block = (t_block *)HEAP_SHIFT(heap);
	while (last_block->next)
		last_block = last_block->next;
	new_block = (t_block *)((char *)BLOCK_SHIFT(last_block) + last_block->data_size);
	new_block->data_size = size;
	new_block->is_free = 0;
	new_block->prev = last_block;
	new_block->next = NULL;
	last_block->next = new_block;
	heap->block_count++;
	heap->free_size -= (size + sizeof(t_block));
	return (new_block);
}
