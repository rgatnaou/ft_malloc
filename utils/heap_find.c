/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_find.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:24:44 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/12 19:03:49 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_heap	*find_heap(size_t size)
{
	t_heap				*heap;
	enum e_heap_type	type;

	type = get_heap_type(size);
	heap = g_heap;
	while (heap)
	{
		if (heap->type == type && heap->free_size >= size + sizeof(t_block))
			return (heap);
		heap = heap->next;
	}
	heap = create_heap(size);
	return (heap);
}
