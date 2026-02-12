/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 15:08:34 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/05 16:18:12 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*start_malloc(size_t size)
{
    t_heap	*heap;
    t_block	*block;

    if (!size)
        return (NULL);
    size = align_size(size);
    block = try_filling_block(size);
    // write(2,"done\n",5);
    if (block)
        return (get_ptr(block));
    // write(2,"done2\n",6);
    heap = find_heap(size);
    if (!heap)
        return (NULL);
    block = append_block(heap, size);
    if (!block)
        return (NULL);
    logs_show();
    return (get_ptr(block));
}

void	*malloc(size_t size)
{
    void	*ptr;

    pthread_mutex_lock(&g_mutex);
    logs_debug("Malloc called with size", size);
    ptr = start_malloc(size);
    if (ptr && getenv_cached(ENV_FILL))
        ft_memset(ptr, 0xaa, size);
    pthread_mutex_unlock(&g_mutex);
    return (ptr);
}