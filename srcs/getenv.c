/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:03:19 by rgatnaou          #+#    #+#             */
/*   Updated: 2026/02/05 16:51:47 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

int	getenv_cached(t_env env)
{
	static int	cached_env_flags;
	static int	cached_initialized;

	if (!cached_initialized)
	{
		if (getenv("Malloc42Debug"))
			cached_env_flags |= ENV_DEBUG;
		if (getenv("Malloc42Show"))
			cached_env_flags |= ENV_SHOW;
		if (getenv("Malloc42FILL"))
			cached_env_flags |= ENV_FILL;
		cached_initialized = 1;
	}
	return (env & cached_env_flags);
}

void	logs_show(void)
{
	if (getenv_cached(ENV_SHOW))
		show_alloc_mem();
}

void	logs_debug(char *msg, size_t size)
{
	int		fd;
	char	*env_file;

	if (!getenv_cached(ENV_DEBUG))
		return ;
	env_file = getenv("Malloc42DebugFile");
	if (env_file)
		fd = open(env_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if(!env_file || fd == -1)
		fd = open(LOGS_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd != -1)
	{
		ft_putstr_fd(msg, fd);
		if (msg[0] != 'F')
		{
			ft_putstr_fd(" : ", fd);
			ft_putnbr_fd(size, fd);
		}
		ft_putstr_fd("\n", fd);
	}
	close(fd);
}
