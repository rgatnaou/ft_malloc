/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:57:31 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/10 18:50:48 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void free(void *ptr)
{
	t_block	*block;
	t_heap	*heap;

	pthread_mutex_lock(&g_mutex);
	heap = g_heap;
	if (!ptr || !heap)
		return ;
	ptr_search(ptr, &heap, &block);
	if (block && heap)
	{
		block->is_free = 1;
		merge_block(heap, block);
		remove_block(heap, block);
		remove_heap(heap);
	}
	pthread_mutex_unlock(&g_mutex);
}
