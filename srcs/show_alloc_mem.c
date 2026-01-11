/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:47:31 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/11 17:47:52 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	show_alloc_mem(void)
{
	t_zone	*zone;
	t_block	*block;

	zone = g_malloc_data.zones;
	while (zone)
	{
		if (zone->type == TINY)
			ft_putstr("TINY : ");
		else if (zone->type == SMALL)
			ft_putstr("SMALL : ");
		else
			ft_putstr("LARGE : ");
		ft_putptr((void *)zone);
		ft_putchar('\n');
		block = zone->blocks;
		while (block)
		{
			if (block->is_free == 0)
			{
				ft_putptr((void *)((char *)block + sizeof(t_block)));
				ft_putstr(" - ");
				ft_putptr((void *)((char *)block + sizeof(t_block) + block->size));
				ft_putstr(" : ");
				ft_putnbr(block->size);
				ft_putstr(" octets\n");
			}
			block = block->next;
		}
		zone = zone->next;
	}
}
