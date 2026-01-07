/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 20:22:21 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/07 20:56:51 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_block	*try_filling_available_block(size_t size)
{
	t_heap	*heap;
	t_block	*block;
	t_block	*new_block;

	find_available_block(&heap, size, &block);
	if (block)
	{
		new_block = (t_block *)((char *)BLOCK_SHIFT(block) + size);
		new_block->data_size = size;
		new_block->is_free = 1;
		new_block->prev = block;
		new_block->next = block->next;

		heap->block_count++;
		block->next = new_block;
		block->data_size = size;
		block->is_free = 0;
		heap->free_size -= block->data_size + sizeof(t_block);
		return (block);
	}
	return (NULL);
}
