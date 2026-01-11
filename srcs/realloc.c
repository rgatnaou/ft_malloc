/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 16:39:26 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/11 17:08:02 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*start_realloc(void *ptr, size_t size, t_heap **heap, t_block **block)
{
	void	*new_ptr;

	ptr_search(ptr, &heap, &block);
	if (!block)
		return (NULL);
	new_ptr = start_malloc(size);
	if (new_ptr)
	{
		ft_memcpy(new_ptr, ptr, block->data_size);
		start_free(ptr, heap, block);
	}
	return (new_ptr);
}

void	*realloc(void *ptr, size_t size)
{
	t_heap	*heap;
	t_block	*block;
	void	*new_ptr;

	if (!ptr)
		return (start_malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	pthread_mutex_lock(&g_mutex);
	new_ptr = start_realloc(ptr, size, &heap, &block);
	pthread_mutex_unlock(&g_mutex);
	return (new_ptr);
}
