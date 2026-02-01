/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:47:31 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/22 18:04:10 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	print_blocks(t_block *block, size_t *total_allocated)
{
	while (block)
	{
		ft_putstr("   ");
		ft_putptr((void *)((char *)block + sizeof(t_block)));
		ft_putstr(" - ");
		ft_putptr((void *)((char *)block + sizeof(t_block)
				+ block->data_size));
		ft_putstr(" : ");
		ft_putnbr(block->data_size);
		ft_putstr(" bytes\n");
		*total_allocated += block->data_size;
		block = block->next;
	}
	ft_putchar('\n');
}

void	show_alloc_mem(void)
{
	t_heap	*heap;
	t_block	*block;
	size_t	total_allocated;

	pthread_mutex_lock(&g_mutex);
	heap = g_heap;
	total_allocated = 0;
	ft_putstr("\n--- Show Alloc Mem ---\n");
	while (heap)
	{
		if (heap->type == TINY)
			ft_putstr("TINY : ");
		else if (heap->type == SMALL)
			ft_putstr("SMALL : ");
		else
			ft_putstr("LARGE : ");
		ft_putptr((void *)heap);
		ft_putchar('\n');
		block = (t_block *)HEAP_SHIFT(heap);
		print_blocks(block, &total_allocated);
		heap = heap->next;
	}
	ft_putstr("Total : ");
	ft_putnbr(total_allocated);
	ft_putstr(" bytes\n");
	ft_putstr("---- end Show Alloc Mem ----\n\n");
	pthread_mutex_unlock(&g_mutex);
}
