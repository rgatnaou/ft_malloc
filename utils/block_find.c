/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_find.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 20:22:09 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/09 18:37:11 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "malloc.h"

void	find_block(size_t s, t_heap **res_heap, t_block **res_block)
{
	t_heap			*heap;
	t_block			*block;
	t_heap_group	group;

	heap = g_heap;
	group = get_heap_type(s);
	while (heap)
	{
		block = HEAP_SHIFT(heap);
		while (block)
		{
			if (heap->type == group && block->is_free
				&& (block->data_size >= s + sizeof(t_block)))
			{
				*res_heap = heap;
				*res_block = block;
				return ;
			}
			block = block->next;
		}
		heap = heap->next;
	}
	*res_block = NULL;
	*res_heap = NULL;
}
