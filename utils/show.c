/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:11:36 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/12 19:06:54 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "stdio.h"

void	show_heap(){
	t_heap *heap = g_heap;
	
	printf("===== HEAP STRUCTURE =====***\n");
	while (heap)
	{
		printf("Heap Type: %s | Total Size: %zu | Free Size: %zu | Block Count: %zu\n",
			heap->type == TINY ? "TINY" : heap->type == SMALL ? "SMALL" : "LARGE",
			heap->total_size,
			heap->free_size,
			heap->block_count);
		t_block *block = (t_block *)HEAP_SHIFT(heap);
		while (block && heap->block_count)
		{
			printf("  Block Size: %zu | Is Free: %d\n",
				block->data_size,
				block->is_free);
			printf("    Data Address: %p\n", BLOCK_SHIFT(block));
			block = block->next;
		}
		heap = heap->next;
	}
	printf("===== END OF HEAP =====***\n");
}

void print_block(t_block *block)
{
	if (block)
	{
		printf("Block at %p: size=%zu, is_free=%d\n", (void *)block, block->data_size, block->is_free);
	}
	else
	{
		printf("Block is NULL..\n");
	}
}

void print_heap(t_heap *heap)
{
	if (heap)
	{
		printf("Heap at %p: type=%d, total_size=%zu, free_size=%zu, block_count=%zu\n",
			(void *)heap, heap->type, heap->total_size, heap->free_size, heap->block_count);
	}
	else
	{
		printf("Heap is NULL\n");
	}
}
