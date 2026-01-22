/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:11:46 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/22 18:01:07 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <limits.h>

void	*calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	total_size;

	if (size != 0 && count > SIZE_MAX / size)
		return (NULL);
	total_size = count * size;
	pthread_mutex_lock(&g_mutex);
	ptr = start_malloc(total_size);
	if (block)
		ft_memset(ptr, 0, size);
	pthread_mutex_unlock(&g_mutex);
	return (ptr);
}
