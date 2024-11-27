/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:38:32 by vberdugo          #+#    #+#             */
/*   Updated: 2024/11/27 14:01:11 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		exec_command(args);
	}
}

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
	wait_for_children();
}

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

void	exec_command(char **args)
{
	if (execvp(args[0], args) == -1)
	{
		perror("execvp");
		exit(EXIT_FAILURE);
	}
}
