/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:56:07 by victor            #+#    #+#             */
/*   Updated: 2024/12/26 13:20:44 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*																			  */
/*                                  										  */
/* ************************************************************************** */
t_expan	init_expan(const char *cmd)
{
	t_expan	exp;

	exp.in_single = 0;
	exp.in_double = 0;
	exp.ind = 0;
	exp.quote_char = '\0';
	exp.cmd_copy = strdup(cmd);
	if (!exp.cmd_copy)
	{
		exp.arg = NULL;
		return (exp);
	}
	exp.arg = NULL;
	exp.ptr = exp.cmd_copy;
	return (exp);
}

/* ************************************************************************** */
/* Closes the given pipe file descriptor if it is valid.                      */
/* ************************************************************************** */
void	close_pipe(int pipefd)
{
	if (pipefd != -1)
	{
		close(pipefd);
	}
}

/* ************************************************************************** */
/* Handles an error in pipe creation by printing an error message and exiting.*/
/* ************************************************************************** */
void	handle_pipe_error(void)
{
	perror("pipe");
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/* Handles an error during process forking by printing an error message and  */
/* exiting the program.                                                       */
/* ************************************************************************** */
void	handle_fork_error(void)
{
	perror("fork");
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/* Attempts to execute an external command using execvp. If execution fails,  */
/* prints an error message and exits the process.                             */
/* ************************************************************************** */
char *find_executable_path(char **paths, char *file)
{
    int i = 0;
    char *temp;
    char *path;

    while (paths[i])
    {
        temp = ft_strjoin(paths[i], "/");
        path = ft_strjoin(temp, file);
        free(temp);
        if (access(path, F_OK | X_OK | R_OK) == 0)
            return (path);
        free(path);
        i++;
    }
    return (NULL);
}

void free_array(char **array)
{
    int i = 0;

    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

char *get_path_from_env(char **env)
{
    char *path = NULL;
    int i = 0;

    while (env[i])
    {
        if (strncmp(env[i], "PATH=", 5) == 0)
        {
            path = strdup(env[i] + 5);
            break;
        }
        i++;
    }
    return path;
}

char *find_command_in_path(char *file, char **env)
{
    char **paths;
    char *path;

    if (file[0] == '/' && access(file, F_OK | X_OK | R_OK) == 0)
    {
        path = strdup(file);
        return (path);
    }
    path = get_path_from_env(env);
    if (!path)
        return (NULL);
    paths = ft_split(path, ':');
    free(path);
    path = find_executable_path(paths, file);
    free_array(paths);
    return (path);
}

void execute_and_handle_error(char **args, char **env)
{
    if (args == NULL || args[0] == NULL) {
        perror("execveNUEVO: No command provided");
        exit(EXIT_FAILURE);
    }

    char *command_path = find_command_in_path(args[0], env);

    if (!command_path) {
        perror("execveERRORAQUI: Command not found");
        exit(EXIT_FAILURE);
    }

    if (execve(command_path, args, env) == -1) {
        perror("execveSEGUNDO: Execve failed");
        free(command_path);
        exit(EXIT_FAILURE);
    }

    free(command_path);
}

