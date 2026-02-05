/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:57:31 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/05 16:51:19 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	start_free(void *ptr)
{
	t_block	*block;
	t_heap	*heap;

	heap = g_heap;
	if (!ptr || !heap)
		return ;
	ptr_search(ptr, &heap, &block);
	if (block)
	{
		block->is_free = 1;
		if (getenv_cached(ENV_FILL))
			ft_memset(ptr, 0x55, block->data_size);
		merge_block(heap, block);
		remove_block(heap, block);
		heap->block_count--;
		remove_heap(heap);
		logs_show();
	}
}

void	free(void *ptr)
{
	pthread_mutex_lock(&g_mutex);
	logs_debug("Free called", 0);
	start_free(ptr);
	pthread_mutex_unlock(&g_mutex);
}
