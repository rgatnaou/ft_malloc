/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 15:08:34 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/12 19:07:21 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdlib.h>

void	*start_malloc(size_t size)
{
	t_heap	*heap;
	t_block	*block;

	if (!size)
		return (NULL);
	size = ALIGN(size);
	block = try_filling_block(size);
	if (block)
		return (BLOCK_SHIFT(block));
	heap = find_heap(size);
	if (!heap)
		return (NULL);
	block = append_block(heap, size);
	return (BLOCK_SHIFT(block));
}

void	*malloc(size_t size)
{
	void	*block;

	pthread_mutex_lock(&g_mutex);
	// log_detail();
	block = start_malloc(size);
	if (block)
		ft_memset(block, 0, size);
	pthread_mutex_unlock(&g_mutex);
	return (block);
}
