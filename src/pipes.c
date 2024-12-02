/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:38:32 by vberdugo          #+#    #+#             */
/*   Updated: 2024/12/02 17:29:55 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* Splits the given command string into tokens (arguments) separated by space */
/* Allocates memory dynamically for each token and returns an array of tokens */
/* ************************************************************************** */
static char	**tokenize_args(char *cmd_copy)
{
	int		c;
	char	**arg;
	char	*token;
	char	*ptr;

	c = 0;
	arg = NULL;
	ptr = cmd_copy;
	token = ft_strsep(&ptr, " ");
	while (token != NULL)
	{
		if (*token != '\0')
		{
			arg = ft_realloc(arg, sizeof(char *) * c, sizeof(char *) * (c + 1));
			if (!arg)
				return (NULL);
			arg[c++] = ft_strdup(token);
		}
		token = ft_strsep(&ptr, " ");
	}
	arg = ft_realloc(arg, sizeof(char *) * c, sizeof(char *) * (c + 1));
	if (!arg)
		return (NULL);
	arg[c] = NULL;
	return (arg);
}

/* ************************************************************************** */
/* Duplicates a command string and splits to arguments using tokenize_args.   */
/* Frees the duplicated command string after tokenization.                    */
/* ************************************************************************** */
char	**split_args(const char *cmd)
{
	char	*cmd_copy;
	char	**args;

	cmd_copy = ft_strdup(cmd);
	if (!cmd_copy)
		return (NULL);
	args = tokenize_args(cmd_copy);
	free(cmd_copy);
	return (args);
}

/* ************************************************************************** */
/* Handles the execution of a single command in a child process.              */
/* Sets up pipe redirections, splits the command into arguments, and executes */
/* built-in commands or external programs.                                    */
/* ************************************************************************** */
void	handle_child(char *sub_t, int prev_fd, int pipefds[2], int *exit_s)
{
	char	**args;

	handle_pipe_redirection(prev_fd, pipefds);
	args = split_args(sub_t);
	if (ft_is_builtin(args[0]))
	{
		ft_exec_builtin(args[0], exit_s);
		exit(*exit_s);
	}
	else
	{
		execute_and_handle_error(args);
	}
}

/* ************************************************************************** */
/* Executes a pipeline of commands separated by pipes.                        */
/* Handles pipe creation, forking processes, and chaining commands through    */
/* pipes. Waits for all child processes to complete.                          */
/* ************************************************************************** */
void	execute_pipeline(char *cmd, int *exit_status)
{
	char	*sub_token;
	int		pipefds[2];
	int		prev_pipefd;
	pid_t	pid;

	prev_pipefd = -1;
	sub_token = strtok(cmd, "|");
	while (sub_token != NULL)
	{
		if (pipe(pipefds) == -1)
			handle_pipe_error();
		pid = fork();
		if (pid == 0)
			handle_child(sub_token, prev_pipefd, pipefds, exit_status);
		else if (pid < 0)
			handle_fork_error();
		close_pipe(prev_pipefd);
		close(pipefds[1]);
		prev_pipefd = pipefds[0];
		sub_token = strtok(NULL, "|");
	}
	close_pipe(prev_pipefd);
	while (wait(NULL) > 0)
		;
}

/* ************************************************************************** */
/* Sets up redirections for pipes. Redirects input from the previous pipe and */
/* output to the current pipe if there are more commands in the pipeline.     */
/* ************************************************************************** */
void	handle_pipe_redirection(int prev_pipefd, int pipefds[2])
{
	if (prev_pipefd != -1)
	{
		dup2(prev_pipefd, STDIN_FILENO);
		close(prev_pipefd);
	}
	if (strtok(NULL, "|") != NULL)
	{
		dup2(pipefds[1], STDOUT_FILENO);
	}
	close(pipefds[0]);
	close(pipefds[1]);
}
