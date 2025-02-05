/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:46:45 by victor            #+#    #+#             */
/*   Updated: 2025/02/05 17:28:16 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_env_arr(char **env)
{
	int		i;
	int		j;
	int		length;
	char	**raw_env;

	i = 0;
	length = 0;
	while (env[length])
		length++;
	raw_env = malloc(sizeof(char *) * (length + 1));
	if (!raw_env)
		return (NULL);
	while (env[i])
	{
		raw_env[i] = ft_strdup(env[i]);
		if (!raw_env[i])
		{
			j = -1;
			while (++j < i)
				free(raw_env[j]);
			return (free(raw_env), NULL);
		}
		i++;
	}
	return (raw_env[i] = NULL, raw_env);
}

// Función para imprimir las variables exportadas
int	print_exported_vars(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_printf("declare -x %s\n", env[i]);
		i++;
	}
	return (0);
}

// Comprobamos que la clave sea válida para una variable de entorno
bool	check_key(char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
	{
		ft_putstr_fd("Not valid identifier\n", 2);
		return (false);
	}
	while (arg[i++])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_' && arg[i] != '\0')
		{
			if (arg[i] != '=')
			{
				ft_putstr_fd("Not valid identifier\n", 2);
				return (false);
			}
			else
				break ;
		}
	}
	return (true);
}

void	remove_env_variable(int index, char **env)
{
	while (env[index])
	{
		env[index] = env[index + 1];
		index++;
	}
}
