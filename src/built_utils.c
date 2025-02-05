/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:03:53 by victor            #+#    #+#             */
/*   Updated: 2025/02/05 17:28:28 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_echo(char **cmd, int *exit_status)
{
	*exit_status = exe_echo(cmd + 1);
}

void	ft_execute_export(char **cmd, char **env)
{
	int	result;

	if (!cmd[1])
	{
		print_exported_vars(env);
		return ;
	}
	result = assign_export_vars(cmd, env);
	if (result)
	{
		ft_putstr_fd("Error: invalid syntax for export\n", 2);
	}
}

void	ft_execute_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_printf("%s\n", env[i]);
		i++;
	}
	return ;
}

void	ft_execute_pwd(int *exit_status)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		write(STDOUT_FILENO, cwd, ft_strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		*exit_status = 0;
	}
	else
	{
		perror("pwd");
		*exit_status = 1;
	}
}

int	ft_execute_unset(char **av, char **env)
{
	int		i;
	int		j;
	char	*key;

	i = -1;
	if (!av[1])
		return (ft_printf("unset: not enough arguments\n"), 1);
	while (av[++i])
	{
		j = 0;
		while (env[j])
		{
			key = ft_substr(env[j], 0, ft_strchr(env[j], '=') - env[j]);
			if (ft_strcmp(av[i], key) == 0)
			{
				free(key);
				free(env[j]);
				remove_env_variable(j, env);
				break ;
			}
			free(key);
			j++;
		}
	}
	return (0);
}
