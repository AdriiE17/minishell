/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:38:32 by vberdugo          #+#    #+#             */
/*   Updated: 2024/12/12 21:00:49 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* Duplicates a command string and splits to arguments using tokenize_args.   */
/* Frees the duplicated command string after tokenization.                    */
/* ************************************************************************** */
char	**split_args(const char *cmd)
{
	t_expan		exp;
	char		*token;

	exp = init_expan(cmd);
	if (!exp.cmd_copy)
		return (NULL);
	token = ft_strsep(&exp.ptr, " ", &exp.in_single, &exp.quote_char);
	while (token != NULL)
	{
		if (*token != '\0')
		{
			exp.arg = ft_realloc(exp.arg, sizeof(char *) * exp.ind,
					sizeof(char *) * (exp.ind + 1));
			if (!exp.arg)
				return (free(exp.cmd_copy), NULL);
			exp.arg[exp.ind++] = ft_strdup(token);
		}
		token = ft_strsep(&exp.ptr, " ", &exp.in_single, &exp.quote_char);
	}
	exp.arg = ft_realloc(exp.arg, sizeof(char *) * exp.ind,
			sizeof(char *) * (exp.ind + 1));
	if (!exp.arg)
		return (free(exp.cmd_copy), NULL);
	return (exp.arg[exp.ind] = NULL, free(exp.cmd_copy), exp.arg);
}

/* ************************************************************************** */
/* Sets up redirections for pipes. Redirects input from the previous pipe and */
/* output to the current pipe if there are more commands in the pipeline.     */
/* ************************************************************************** */
void	handle_pipe_redirection(int prev_pipe, int pipefds[2], int next_cmd)
{
	if (prev_pipe != -1)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (next_cmd)
	{
		dup2(pipefds[1], STDOUT_FILENO);
	}
	close(pipefds[0]);
	close(pipefds[1]);
}

/* ************************************************************************** */
/* Handles the execution of a single command in a child process.              */
/* Sets up pipe redirections, splits the command into arguments, and executes */
/* built-in commands or external programs.                                    */
/* ************************************************************************** */
void	handle_child(char *sub_t, t_pipe *pipe_d, int *exit_s)
{
	char	**args;

	args = split_args(sub_t);
	handle_pipe_redirection(pipe_d->pre_fd, pipe_d->pipefds, pipe_d->has_cmd);
	handle_redirections(args, exit_s);
	if (ft_is_builtin(args[0]))
	{
		ft_exec_builtin(args[0], exit_s);
		exit(*exit_s);
	}
	else
		execute_and_handle_error(args);
}

/* ************************************************************************** */
/* Executes a pipeline of commands separated by pipes.                        */
/* Handles pipe creation, forking processes, and chaining commands through    */
/* pipes. Waits for all child processes to complete.                          */
/* ************************************************************************** */
void	handle_pipeline(char *cmd, int *exit_status, t_pipe *pip)
{
	pid_t	pid;

	pip->in_q = 0;
	pip->q_char = '\0';
	pip->sub_token = ft_strsep(&cmd, "|", &pip->in_q, &pip->q_char);
	while (pip->sub_token != NULL)
	{
		pip->has_cmd = (cmd != NULL && *cmd != '\0');
		if (pipe(pip->pipefds) == -1)
			handle_pipe_error();
		pid = fork();
		if (pid == 0)
			handle_child(pip->sub_token, pip, exit_status);
		else if (pid < 0)
			handle_fork_error();
		close_pipe(pip->pre_fd);
		close(pip->pipefds[1]);
		pip->pre_fd = pip->pipefds[0];
		if (pip->has_cmd)
			pip->sub_token = ft_strsep(&cmd, "|", &pip->in_q, &pip->q_char);
		else
			break ;
	}
}

void	execute_pipeline(char *cmd, int *exit_status)
{
	t_pipe	pipe;

	pipe.pre_fd = -1;
	handle_pipeline(cmd, exit_status, &pipe);
	close_pipe(pipe.pre_fd);
	while (wait(NULL) > 0)
		;
}
