/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:47:31 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/12 15:19:44 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	show_alloc_mem(void)
{
	t_heap	*heap;
	t_block	*block;

	heap = g_heap;
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
		while (block)
		{
			if (block->is_free == 0)
			{
				ft_putptr((void *)((char *)block + sizeof(t_block)));
				ft_putstr(" - ");
				ft_putptr((void *)((char *)block + sizeof(t_block) + block->data_size));
				ft_putstr(" : ");
				ft_putnbr(block->data_size);
				ft_putstr(" octets\n");
			}
			block = block->next;
		}
		heap = heap->next;    
	}
}
