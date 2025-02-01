/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:03:53 by victor            #+#    #+#             */
/*   Updated: 2025/02/01 13:50:27 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_echo(char **cmd, int *exit_status)
{
	*exit_status = blt_echo(cmd + 1);
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

void	ft_execute_unset(char **av, char **env)
{
	blt_unset(av, env);
}

void	ft_execute_env(char **env)
{
	blt_env(env);
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
