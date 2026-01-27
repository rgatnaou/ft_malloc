/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem_ex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:47:31 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/22 18:03:05 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	print_hex_dump(const void *addr, size_t size)
{
	const unsigned char	*data;
	size_t				i;
	size_t				j;

	data = (const unsigned char *)addr;
	i = 0;
	while (i < size)
	{
		ft_putstr("  ");
		ft_putnbr(i);
		ft_putstr(" : ");
		j = 0;
		while (j < 16)
		{
			if (i + j < size)
			{
				ft_putchar("0123456789abcdef"[data[i + j] / 16]);
				ft_putchar("0123456789abcdef"[data[i + j] % 16]);
			}
			ft_putchar(' ');
			j++;
		}
		ft_putstr("| ");
		j = 0;
		while (j < 16 && i + j < size)
		{
			if (data[i + j] >= 32 && data[i + j] <= 126)
				ft_putchar(data[i + j]);
			else
				ft_putchar('.');
			j++;
		}
		ft_putstr(" |\n");
		i += 16;
	}
}

static void	print_blocks(t_block *block, size_t *total_allocated)
{
	while (block)
	{
		if (block->is_free == 0)
			ft_putstr(" [USED] ");
		else
			ft_putstr(" [FREE] ");
		ft_putptr((void *)((char *)block + sizeof(t_block)));
		ft_putstr(" - ");
		ft_putptr((void *)((char *)block + sizeof(t_block)
				+ block->data_size));
		ft_putstr(" : ");
		ft_putnbr(block->data_size);
		ft_putstr(" bytes\n");
		ft_putstr("  Hex dump: \n");
		if (block->is_free == 0)
		{
			*total_allocated += block->data_size;
			print_hex_dump((char *)block + sizeof(t_block), block->data_size);
			ft_putchar('\n');
		}
		else
			ft_putstr("   Free block, no hex dump.\n\n");
		block = block->next;
	}
	ft_putchar('\n');
}

void	show_alloc_mem_ex(void)
{
	t_heap	*heap;
	t_block	*block;
	size_t	total_allocated;

	pthread_mutex_lock(&g_mutex);
	heap = g_heap;
	total_allocated = 0;
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
		ft_putstr("total blocks : ");
		ft_putnbr(heap->block_count);
		ft_putchar('\n');
		heap = heap->next;
	}
	ft_putstr("Total : ");
	ft_putnbr(total_allocated);
	ft_putstr(" bytes\n");

	ft_putstr("----- End of detailed memory allocation -----\n");
	pthread_mutex_unlock(&g_mutex);
}
