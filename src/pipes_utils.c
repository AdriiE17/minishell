/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:56:07 by victor            #+#    #+#             */
/*   Updated: 2025/02/05 17:59:28 by victor           ###   ########.fr       */
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
/* Attempts to execute an external command using execve. If execution fails,  */
/* prints an error message and exits the process.                             */
/* ************************************************************************** */
void	execute_and_handle_error(char **args, char **env)
{
	char	*command_path;

	command_path = find_command_in_path(args[0], env);
	if (!command_path)
	{
		perror("execve: Command not found");
		exit(EXIT_FAILURE);
	}
	if (execve(command_path, args, env) == -1)
	{
		perror("execve: Execve failed");
		free(command_path);
		exit(EXIT_FAILURE);
	}
	free(command_path);
}
