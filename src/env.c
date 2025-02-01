/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:46:45 by victor            #+#    #+#             */
/*   Updated: 2025/02/01 16:28:01 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	blt_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_printf("%s\n", env[i]);
		i++;
	}
	return (0);
}

int	blt_unset(char **av, char **env)
{
	int		i;
	int		j;
	int		k;
	char	*key;

	i = 1;
	if (!av[1])
		return (ft_printf("unset: not enough arguments\n"), 1);
	while (av[i])
	{
		j = 0;
		while (env[j])
		{
			key = ft_substr(env[j], 0, ft_strchr(env[j], '=') - env[j]);
			if (ft_strcmp(av[i], key) == 0)
			{
				free(key);
				free(env[j]);
				k = j;
				while (env[k])
				{
					env[k] = env[k + 1];
					k++;
				}
				break ;
			}
			free(key);
			j++;
		}
		i++;
	}
	return (0);
}
