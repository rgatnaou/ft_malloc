/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_find.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 20:22:09 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/04 15:27:49 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	find_block(size_t s, t_heap **res_heap, t_block **res_block)
{
    t_heap			*heap;
    t_block			*block;
    t_heap_group	group;

    // write(2, "find_block: s=", 13);
    // ft_putnbr_fd(s, 2);
    group = get_heap_type(s);
    // write(2, " group=", 7);
    // ft_putnbr_fd(group, 2);
    // write(2, "\n", 1);
    heap = g_heap;
    while (heap)
    {
        // write(2, "heap type=", 10);
        // ft_putnbr_fd(heap->type, 2);
        // write(2, " block_count=", 13);
        // ft_putnbr_fd(heap->block_count, 2);
        // write(2, " total_size=", 12);
        // ft_putnbr_fd(heap->total_size, 2);
        // write(2, "\n", 1);
        if (heap->block_count == 0)
        {
            heap = heap->next;
            continue;
        }
        block = heap_shift(heap);
        // write(2, "checking blocks\n", 16);
        while (heap->type == group && block)
        {
            // Prevent accessing blocks outside the heap
            if ((char *)block < (char *)heap + sizeof(t_heap) ||
                (char *)block >= (char *)heap + heap->total_size)
            {
                write(2, "block out of bounds\n", 20);
                break;
            }
            // write(2, "block data_size=", 16);
            // ft_putnbr_fd(block->data_size, 2);
            // write(2, " is_free=", 9);
            // ft_putnbr_fd(block->is_free, 2);
            // write(2, "\n", 1);
            if (block->is_free
                && (block->data_size >= s))
            {
                *res_heap = heap;
                *res_block = block;
                return ;
            }
            block = block->next;
        }
        heap = heap->next;
    }
    *res_block = NULL;
    *res_heap = NULL;
}
// ...existing code...