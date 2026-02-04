/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ptr_search.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 19:47:58 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/04 18:53:37 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	ptr_search(void *ptr, t_heap **ptr_heap, t_block **ptr_block)
{
	t_heap	*heap;
	t_block	*block;

	heap = g_heap;
	while (heap)
	{
		block = heap_shift(heap);
		while (block)
		{
			if (get_ptr(block) == ptr)
			{
				*ptr_block = block;
				*ptr_heap = heap;
				return ;
			}
			block = block->next;
		}
		heap = heap->next;
	}
	*ptr_heap = NULL;
	*ptr_block = NULL;
}
