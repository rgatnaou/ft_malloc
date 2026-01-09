/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   size.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 20:18:17 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/09 17:23:38 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

int	get_heap_size(size_t size)
{
	if (size <= (size_t)TINY_BLOCK_MAX)
		return (TINY_HEAP_SIZE);
	else if (size <= (size_t)SMALL_BLOCK_MAX)
		return (SMALL_HEAP_SIZE);
	else
		return (ALIGN(size + sizeof(t_heap) + sizeof(t_block)));
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
