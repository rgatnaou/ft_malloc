/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_remove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 18:28:03 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/10 18:35:59 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	remove_block(t_heap *heap, t_block *block)
{
	if (block->is_free && !block->next)
	{
		if (block->prev)
			block->prev->next = NULL;
		heap->free_size += block->data_size + sizeof(t_block);
		heap->block_count--;
	}
}
