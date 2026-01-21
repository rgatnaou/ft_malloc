/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:39:26 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/21 19:48:10 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*start_realloc(void *ptr, size_t size)
{
	t_block	*block;
	t_heap	*heap;
	void	*new_ptr;

	heap = g_heap;
	block = NULL;
	ptr_search(ptr, &heap, &block);
	if (!block)
		return (NULL);
	new_ptr = start_malloc(size);
	if (new_ptr)
	{
		ft_memcpy(new_ptr, ptr, block->data_size);
		start_free(heap, block);
	}
	return (new_ptr);
}

void	*realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (!ptr)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	pthread_mutex_lock(&g_mutex);
	new_ptr = start_realloc(ptr, size);
	pthread_mutex_unlock(&g_mutex);
	return (new_ptr);
}
