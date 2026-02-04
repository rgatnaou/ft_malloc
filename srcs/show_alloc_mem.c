/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:47:31 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/04 19:18:35 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	print_blocks(t_block *block, size_t *t_allocated, size_t *t_blocks)
{
	while (block && !block->is_free)
	{
		ft_putstr("\n   ");
		ft_putptr(get_ptr(block));
		ft_putstr(" - ");
		ft_putptr(end_of_ptr(block));
		ft_putstr(" : ");
		ft_putnbr(block->data_size);
		ft_putstr(" bytes");
		*t_allocated += block->data_size;
		(*t_blocks)++;
		block = block->next;
	}
	ft_putchar('\n');
}

void	print_group(t_heap *heap)
{
	if (heap->type == TINY)
		ft_putstr("\nTINY : ");
	else if (heap->type == SMALL)
		ft_putstr("SMALL : ");
	else
		ft_putstr("LARGE : ");
	ft_putptr((void *)heap);
	// ft_putchar('\n');
}

void	print_summary(size_t t_allocated, size_t t_blocks, size_t t_heaps)
{
	if (t_allocated)
	{
		ft_putstr("\nTotal heaps  : ");
		ft_putnbr(t_heaps);
		ft_putstr("\n");
		ft_putstr("Total blocks : ");
		ft_putnbr(t_blocks);
		ft_putstr("\n");
		ft_putstr("Total bytes  : ");
		ft_putnbr(t_allocated);
		ft_putstr("\n");
	}
	else
		ft_putstr("   No allocation found\n");
}

void	show_alloc_mem(void)
{
	t_heap	*heap;
	t_block	*block;
	size_t	total_allocated;
	size_t	total_blocks;
	size_t	total_heaps;

	pthread_mutex_lock(&g_mutex);
	heap = g_heap;
	total_allocated = 0;
	total_blocks = 0;
	total_heaps = 0;
	ft_putstr("\n--- Show Alloc Mem ---\n");
	while (heap)
	{
		print_group(heap);
		block = heap_shift(heap);
		print_blocks(block, &total_allocated, &total_blocks);
		heap = heap->next;
		total_heaps++;
	}
	print_summary(total_allocated, total_blocks, total_heaps);
	ft_putstr("---- end Show Alloc Mem ----\n\n");
	pthread_mutex_unlock(&g_mutex);
}
