/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 20:18:17 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/07 20:22:17 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

int	get_heap_size(size_t size)
{
	if (size <= TINY_BLOCK_MAX)
		return (TINY_HEAP_SIZE);
	else if (size <= SMALL_BLOCK_MAX)
		return (SMALL_HEAP_SIZE);
	else
		return (ALIGN(size + sizeof(t_heap) + sizeof(t_block)));
}

t_heap_group get_heap_group(size_t size)
{
	if (size <= TINY_BLOCK_MAX)
		return (TINY);
	else if (size <= SMALL_BLOCK_MAX)
		return (SMALL);
	else
		return (LARGE);
}
