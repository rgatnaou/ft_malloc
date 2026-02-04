/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem_ex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:47:31 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/04 21:38:29 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	print_chars(unsigned char *data, int size, int offset)
{
	int	j;

	ft_putstr("| ");
	j = 0;
	while (j < 16 && offset + j < size)
	{
		if (data[offset + j] >= 32 && data[offset + j] <= 126)
			ft_putchar(data[offset + j]);
		else
			ft_putchar('.');
		j++;
	}
	ft_putstr(" |\n");
}

static void	print_hex_dump(const void *addr, size_t size)
{
	const unsigned char	*data;
	size_t				i;
	size_t				j;
	size_t				limit;

	data = (const unsigned char *)addr;
	i = 0;
	limit = size > 256 ? 256 : size;
	while (i < limit)
	{
		ft_putstr("  ");
		ft_putnbr(i);
		ft_putstr(" : ");
		j = 0;
		while (j < 16)
		{
			if (i + j < limit)
			{
				ft_putchar("0123456789abcdef"[data[i + j] / 16]);
				ft_putchar("0123456789abcdef"[data[i + j] % 16]);
			}
			ft_putchar(' ');
			j++;
		}
		print_chars((unsigned char *)data, limit, i);
		i += 16;
	}
}

static void	print_blocks(t_block *block, size_t *t_allocated, size_t *t_blocks)
{
	while (block)
	{
		if (block->is_free)
			ft_putstr("\n [FREE] ");
		else
			ft_putstr("\n [USED] ");
		ft_putptr((void *)block);
		ft_putstr(" : ");
		ft_putptr(get_ptr(block));
		ft_putstr(" - ");
		ft_putptr(end_of_ptr(block));
		ft_putstr(" : ");
		ft_putnbr(block->data_size);
		ft_putstr(" bytes\n  Hex dump: \n");
		if (block->is_free == 0)
		{
			*t_allocated += block->data_size;
			print_hex_dump((char *)block + sizeof(t_block), block->data_size);
			ft_putchar('\n');
			(*t_blocks)++;
		}
		else
			ft_putstr("   Free block, no hex dump.\n\n");
		block = block->next;
	}
}

void	show_alloc_mem_ex(void)
{
	t_heap	*heap;
	size_t	total_allocated;
	size_t	total_blocks;
	size_t	total_heaps;

	pthread_mutex_lock(&g_mutex);
	heap = g_heap;
	total_allocated = 0;
	total_blocks = 0;
	total_heaps = 0;
	ft_putstr("\n----- Detailed memory allocation Mem_ex -----\n");
	while (heap && ++total_heaps)
	{
		print_group(heap);
		print_blocks(heap_shift(heap), &total_allocated, &total_blocks);
		ft_putstr("Total blocks used: ");
		ft_putnbr(heap->block_count);
		ft_putstr(" blocks\n\n");
		heap = heap->next;
	}
	print_summary(total_allocated, total_blocks, total_heaps);
	ft_putstr("----- End of detailed memory allocation Mem_ex -----\n\n");
	pthread_mutex_unlock(&g_mutex);
}
