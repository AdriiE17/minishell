/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:34:47 by victor            #+#    #+#             */
/*   Updated: 2024/12/06 11:27:02 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* Determines if the given command is a built-in shell command. It compares   */
/* the input string with a predefined list of built-in commands and returns   */
/* 1 if it's a built-in command, otherwise returns 0.                         */
/* ************************************************************************** */
int	ft_is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0)
		return (1);
	return (0);
}

/* ************************************************************************** */
/* Handles the execution of built-in shell commands like cd, exit, echo,      */
/* pwd, export, unset, and env. Based on the command, it performs the         */
/* appropriate action, such as changing the directory, printing the current   */
/* directory, setting/unsetting environment variables, or printing environment*/
/* variables. The exit status is updated accordingly for each command.        */
/* ************************************************************************** */
void	ft_exec_builtin(char *cmd, int *exit_status)
{
	if (ft_strcmp(cmd, "cd") == 0)
		ft_execute_cd(exit_status);
	else if (ft_strcmp(cmd, "echo") == 0)
		ft_execute_echo(exit_status);
	else if (ft_strcmp(cmd, "export") == 0)
		ft_execute_export(exit_status);
	else if (ft_strcmp(cmd, "unset") == 0)
		ft_execute_unset(exit_status);
	else if (ft_strcmp(cmd, "env") == 0)
		ft_execute_env(exit_status);
	else if (ft_strcmp(cmd, "pwd") == 0)
		ft_execute_pwd(exit_status);
}

void	ft_execute_cd(int *exit_status)
{
	char	*path;

	path = ft_strtok(NULL, " ");
	(void)exit_status;
	if (path == NULL)
	{
		printf("cd ejecutado desde ft_execute_cd\n");
	}
}
