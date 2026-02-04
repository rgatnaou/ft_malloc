/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   size.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 20:18:17 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/04 17:39:27 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

int	get_heap_size(size_t size)
{
	if (size <= (size_t)TINY_BLOCK_MAX)
		return (heap_size(4));
	else if (size <= (size_t)SMALL_BLOCK_MAX)
		return (heap_size(32));
	else
		return (align_size(size + sizeof(t_heap) + sizeof(t_block)));
}

t_heap_group	get_heap_type(size_t size)
{
	if (size <= (size_t)TINY_BLOCK_MAX)
		return (TINY);
	else if (size <= (size_t)SMALL_BLOCK_MAX)
		return (SMALL);
	else
		return (LARGE);
}
