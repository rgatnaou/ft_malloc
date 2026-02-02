#include "malloc.h"


int	getenv_cached(t_env env)
{
	static int	cached_env_flags;
	static int cached_initialized;

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

void	logs_show()
{
	if (getenv_cached(ENV_SHOW)){
		show_alloc_mem();
	}
}

void	logs_debug(char *msg, size_t size)
{
	if (getenv_cached(ENV_DEBUG)){
		ft_putstr(msg);
		if (msg[0] == 'F')
		{
			ft_putstr(" : ");
			ft_putnbr(size);
		}
		ft_putstr("\n");
	}
}
