/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:39:26 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/04 15:26:33 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*start_realloc(void *ptr, size_t size)
{
	t_block	*block;
	t_heap	*heap;
	void	*new_ptr;
	size_t	copy_size;

	heap = g_heap;
	block = NULL;
	if (!ptr)
		return (start_malloc(size));
	else if (size == 0)
	{
		start_free(ptr);
		return (NULL);
	}
	ptr_search(ptr, &heap, &block);
	if (!heap || !block)
		return (NULL);
	else if (align_size(size) == block->data_size)
		return (ptr);
	new_ptr = start_malloc(size);
	if (!new_ptr)
		return (NULL);
	copy_size = block->data_size;
	if (align_size(size) < copy_size)
		copy_size = align_size(size);
	ft_memmove(new_ptr, ptr, copy_size);
	start_free(ptr);
	return (new_ptr);
}

void	*realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	pthread_mutex_lock(&g_mutex);
	logs_debug("Realloc called with size", size);
	if (size == 0)
	{
		pthread_mutex_unlock(&g_mutex);
		free(ptr);
		return (NULL);
	}
	if (!ptr)
	{
		new_ptr = start_malloc(size);
		pthread_mutex_unlock(&g_mutex);
		return (new_ptr);
	}
	new_ptr = start_realloc(ptr, size);
	pthread_mutex_unlock(&g_mutex);
	return (new_ptr);
}
