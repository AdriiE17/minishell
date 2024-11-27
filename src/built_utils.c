/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:03:53 by victor            #+#    #+#             */
/*   Updated: 2024/11/27 12:09:20 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_echo(int *exit_status)
{
	char	*arg;

	arg = ft_strtok(NULL, " ");
	while (arg != NULL)
	{
		write(STDOUT_FILENO, arg, ft_strlen(arg));
		write(STDOUT_FILENO, " ", 1);
		arg = ft_strtok(NULL, " ");
	}
	write(STDOUT_FILENO, "\n", 1);
	*exit_status = 0;
}

void	ft_execute_export(int *exit_status)
{
	char	*var;
	char	*value;

	var = ft_strtok(NULL, " ");
	if (var)
	{
		value = ft_strtok(NULL, " ");
		if (value)
		{
			setenv(var, value, 1);
			*exit_status = 0;
		}
		else
		{
			write(STDERR_FILENO, "export: invalid argument\n", 25);
			*exit_status = 1;
		}
	}
}

void	ft_execute_unset(int *exit_status)
{
	char	*var;

	var = ft_strtok(NULL, " ");
	if (var)
	{
		unsetenv(var);
		*exit_status = 0;
	}
	else
	{
		write(STDERR_FILENO, "unset: invalid argument\n", 24);
		*exit_status = 1;
	}
}

void	ft_execute_env(int *exit_status)
{
	extern char	**environ;
	int			i;

	i = -1;
	while (environ[++i] != NULL)
	{
		write(STDOUT_FILENO, environ[i], ft_strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
	}
	*exit_status = 0;
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
