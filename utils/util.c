/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:54:42 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/04 18:53:56 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

size_t	heap_size(int pages)
{
	return (pages * getpagesize());
}

size_t	align_size(size_t size)
{
	return ((size + 15) & ~15);
}

void	*get_ptr(t_block *ptr)
{
	return ((void *)((char *)ptr + sizeof(t_block)));
}

t_block	*heap_shift(t_heap *ptr)
{
	return ((void *)((char *)ptr + sizeof(t_heap)));
}

void	*end_of_ptr(t_block *ptr)
{
	return ((void *)((char *)ptr + sizeof(t_block) + ptr->data_size));
}
