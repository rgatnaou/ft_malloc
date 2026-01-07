/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 18:46:21 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/07 20:01:47 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

pthread_mutex_t		g_mutex = PTHREAD_MUTEX_INITIALIZER;
t_heap				*g_heap = NULL;
