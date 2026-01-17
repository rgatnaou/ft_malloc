/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:58:32 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/12 18:00:55 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putptr(void *ptr)
{
	size_t			address;
	char			hex_chars[17];
	char			hex_str[2 + sizeof(size_t) * 2 + 1];
	size_t			i;

	ft_memcpy(hex_chars, "0123456789abcdef", 17);
	address = (size_t)ptr;
	hex_str[0] = '0';
	hex_str[1] = 'x';
	i = 2 + sizeof(size_t) * 2;
	hex_str[i] = '\0';
	while (i > 2)
	{
		hex_str[--i] = hex_chars[address % 16];
		address /= 16;
	}
	ft_putstr(hex_str);
}
