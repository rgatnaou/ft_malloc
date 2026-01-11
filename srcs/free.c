/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:57:31 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/11 17:03:03 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	start_free(void *ptr, t_heap *heap, t_block *block)
{
	if (block)
	{
		block->is_free = 1;
		merge_block(heap, block);
		remove_block(heap, block);
		remove_heap(heap);
	}
}

void free(void *ptr)
{
	t_block	*block;
	t_heap	*heap;

	heap = g_heap;
	if (!ptr || !heap)
		return ;
	ptr_search(ptr, &heap, &block);
	pthread_mutex_lock(&g_mutex);
	start_free(ptr, heap, block);
	pthread_mutex_unlock(&g_mutex);
}
