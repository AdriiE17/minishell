/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:34:47 by victor            #+#    #+#             */
/*   Updated: 2025/01/31 11:52:21 by victor           ###   ########.fr       */
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
void ft_exec_builtin(char **cmd, int *exit_status, char **env)
{
    if (ft_strcmp(cmd[0], "cd") == 0)
        ft_execute_cd(cmd, env);
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

    for (int i = 0; cmd[i] != NULL; i++)
    {
        free(cmd[i]); 
    }
    free(cmd); 
}

void ft_execute_cd(char **av, char **env)
{
    static char oldpwd[PATH_MAX] = "";
    char *path;
    int result;
    char buf[PATH_MAX];
    char *tmp;
    (void)env;

    result = 0;
    if (!av[1])
    {
        path = getenv("HOME");
        if (!path)
        {
            printf("cd: HOME not set\n");
            return;
        }
    }
    else if (strcmp(av[1], "-") == 0) // Si el usuario escribe "cd -"
    {
        if (oldpwd[0] == '\0') // Si oldpwd no está inicializado
        {
            printf("cd: OLDPWD not set\n");
            return;
        }
        printf("%s\n", oldpwd); // Mostrar el directorio al que cambiamos
        path = oldpwd;
    }
    else
    {
        path = av[1];
    }

    if (chdir(path) != 0)
    {
        perror("cd");
        result = 1;
    }
    if (!result)
    {
        tmp = getenv("PWD");
        if (tmp)
        {
            strncpy(oldpwd, tmp, PATH_MAX - 1); // Guardar PWD en oldpwd
            oldpwd[PATH_MAX - 1] = '\0'; // Asegurar el null-terminator
        }
        if (getcwd(buf, sizeof(buf)) != NULL)
        {
            setenv("PWD", buf, 1);
        }
    }
}

