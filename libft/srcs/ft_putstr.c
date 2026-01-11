/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:50:19 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/11 18:06:44 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		ft_putchar((unsigned char)str[i]);
		i++;
	}
}
