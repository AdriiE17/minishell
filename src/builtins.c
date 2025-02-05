/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:34:47 by victor            #+#    #+#             */
/*   Updated: 2025/02/05 17:17:44 by victor           ###   ########.fr       */
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
void	ft_exec_builtin(char **cmd, int *exit_status, char **env)
{
	int	i;

	i = -1;
	if (ft_strcmp(cmd[0], "cd") == 0)
		ft_execute_cd(cmd);
	else if (ft_strcmp(cmd[0], "echo") == 0)
		ft_execute_echo(cmd, exit_status);
	else if (ft_strcmp(cmd[0], "export") == 0)
		ft_execute_export(cmd, env);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		ft_execute_unset(cmd, env);
	else if (ft_strcmp(cmd[0], "env") == 0)
		ft_execute_env(env);
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		ft_execute_pwd(exit_status);
	while (cmd[++i] != NULL)
	{
		free(cmd[i]);
	}
	free(cmd);
}

static char	*get_target_path(char **av, char *oldpwd)
{
	char	*home;

	if (!av[1])
	{
		home = getenv("HOME");
		if (!home)
		{
			printf("cd: HOME not set\n");
			return (NULL);
		}
		return (home);
	}
	if (strcmp(av[1], "-") == 0)
	{
		if (oldpwd[0] == '\0')
		{
			printf("cd: OLDPWD not set\n");
			return (NULL);
		}
		printf("%s\n", oldpwd);
		return (oldpwd);
	}
	return (av[1]);
}

static void	update_directories(char *oldpwd)
{
	char	buf[PATH_MAX];
	char	*tmp;

	tmp = getenv("PWD");
	if (tmp)
	{
		strncpy(oldpwd, tmp, PATH_MAX - 1);
		oldpwd[PATH_MAX - 1] = '\0';
	}
	if (getcwd(buf, sizeof(buf)) != NULL)
	{
		setenv("PWD", buf, 1);
	}
}

void	ft_execute_cd(char **av)
{
	static char	oldpwd[PATH_MAX] = "";
	char		*path;

	path = get_target_path(av, oldpwd);
	if (!path)
		return ;
	if (chdir(path) != 0)
	{
		perror("cd");
		return ;
	}
	update_directories(oldpwd);
}
