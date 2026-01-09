/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_append.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:21:08 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/09 16:28:35 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	append_block(t_heap *heap, t_block *block, size_t size)
{
	t_block	*last_block;

	last_block = (t_block *)HEAP_SHIFT(heap);
	while (last_block->next)
		last_block = last_block->next;
	last_block->next = block;
	block->prev = last_block;
	block->next = NULL;
	block->data_size = size;
	block->is_free = 0;
	heap->block_count++;
	heap->free_size -= (size + sizeof(t_block));
}
