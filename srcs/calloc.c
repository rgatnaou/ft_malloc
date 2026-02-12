/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:11:46 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/05 16:17:56 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	total_size;

	pthread_mutex_lock(&g_mutex);
	if (size != 0 && count > SIZE_MAX / size)
	{
		pthread_mutex_unlock(&g_mutex);
		return (NULL);
	}
	total_size = count * size;
	logs_debug("Calloc called with size", total_size);
	ptr = start_malloc(total_size);
	if (ptr)
		ft_memset(ptr, 0, total_size);
	pthread_mutex_unlock(&g_mutex);
	return (ptr);
}
