/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:58:32 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/01/22 16:41:35 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putptr(void *ptr)
{
	size_t			address;
	char			*hex_chars;
	char			hex_str[2 + sizeof(size_t) * 2 + 1];
	size_t			i;

	hex_chars = "0123456789abcdef";
	address = (size_t)ptr;
	ft_putstr("0x");
	i = 0;
	hex_str[i] = '\0';
	while (address)
	{
		hex_str[++i] = hex_chars[address % 16];
		address /= 16;
	}
	while (i > 0)
		ft_putchar(hex_str[i--]);
}
