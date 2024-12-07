/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:07:58 by vberdugo          #+#    #+#             */
/*   Updated: 2024/12/06 22:34:51 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_arguments(char **args, int *exit_status)
{
	int		i;
	char	*expanded;

	i = 0;
	while (args[i])
	{
		expanded = exp_env_vars(args[i], *exit_status);
		if (!expanded)
		{
			fprintf(stderr, "Error: failed to expand variables\n");
			*exit_status = 1;
			return ;
		}
		free(args[i]);
		args[i] = expanded;
		i++;
	}
}

void	handle_here_doc_input(int pipe_fd[2], char *delimiter)
{
	char	*line;
	size_t	len;

	line = NULL;
	len = 0;
	while (1)
	{
		printf("> ");
		if (getline(&line, &len, stdin) == -1)
			break ;
		line[strcspn(line, "\n")] = '\0';
		if (strcmp(line, delimiter) == 0)
			break ;
		write(pipe_fd[1], line, strlen(line));
		write(pipe_fd[1], "\n", 1);
	}
	free(line);
	close(pipe_fd[1]);
	exit(0);
}

void	handle_input_redirection(char **args, int *exit_status, int *i)
{
	int	fd_in;

	fd_in = open(args[*i + 1], O_RDONLY);
	if (fd_in == -1)
	{
		perror(args[*i + 1]);
		*exit_status = 1;
		return ;
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd_in);
		*exit_status = 1;
		return ;
	}
	close(fd_in);
	args[*i] = NULL;
	args[*i + 1] = NULL;
	(*i)++;
}

void	handle_output_redirection(char **args, int *exit_status, int *i)
{
	int	fd_out;
	int	flags;

	if (strcmp(args[*i], ">>") == 0)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd_out = open(args[*i + 1], flags, 0644);
	if (fd_out == -1)
	{
		perror(args[*i + 1]);
		*exit_status = 1;
		return ;
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd_out);
		*exit_status = 1;
		return ;
	}
	close(fd_out);
	args[*i] = NULL;
	args[*i + 1] = NULL;
	(*i)++;
}

void	handle_here_doc_redirection(char **args, int *exit_status, int *i)
{
	char	*delimiter;
	int		pipe_fd[2];
	pid_t	pid;

	delimiter = args[*i + 1];
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		*exit_status = 1;
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		handle_here_doc_input(pipe_fd, delimiter);
	}
	else if (pid > 0)
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(pipe_fd[0]);
			*exit_status = 1;
			return ;
		}
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
	else
	{
		perror("fork");
		*exit_status = 1;
		return ;
	}
	args[*i] = NULL;
	args[*i + 1] = NULL;
	(*i)++;
}

void	handle_redirections(char **args, int *exit_status)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (strcmp(args[i], "<") == 0)
		{
			if (args[i + 1])
				handle_input_redirection(args, exit_status, &i);
			else
			{
				fprintf(stderr, "Error: missing filename after '<'\n");
				*exit_status = 1;
				return ;
			}
		}
		else if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0)
		{
			if (args[i + 1])
				handle_output_redirection(args, exit_status, &i);
			else
			{
				fprintf(stderr, "Error: miss file after '%s'\n", args[i]);
				*exit_status = 1;
				return ;
			}
		}
		else if (strcmp(args[i], "<<") == 0)
		{
			if (args[i + 1])
				handle_here_doc_redirection(args, exit_status, &i);
			else
			{
				fprintf(stderr, "Error: missing delimiter after '<<'\n");
				*exit_status = 1;
				return ;
			}
		}
		i++;
	}
}

/*
void	handle_redirections(char **args, int *exit_status)
{
	int	i;
	int	fd_in;
	int	fd_out;

	i = 0;
	fd_in = -1;
	fd_out = -1;
	while (args[i])
	{
		if (strcmp(args[i], "<") == 0)
		{
			if (args[i + 1])
			{
				fd_in = open(args[i + 1], O_RDONLY);
				if (fd_in == -1)
				{
					perror(args[i + 1]);
					*exit_status = 1;
					return ;
				}
				if (dup2(fd_in, STDIN_FILENO) == -1)
				{
					perror("dup2");
					close(fd_in);
					*exit_status = 1;
					return ;
				}
				close(fd_in);
				args[i] = NULL;
				args[i + 1] = NULL;
				i++;
			}
			else
			{
				fprintf(stderr, "Error: missing filename after '<'\n");
				*exit_status = 1;
				return ;
			}
		}
		else if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0)
		{
	if (strcmp(args[*i], ">>") == 0)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
			if (args[i + 1])
			{
				fd_out = open(args[i + 1], flags, 0644);
				if (fd_out == -1)
				{
					perror(args[i + 1]);
					*exit_status = 1;
					return ;
				}
				if (dup2(fd_out, STDOUT_FILENO) == -1)
				{
					perror("dup2");
					close(fd_out);
					*exit_status = 1;
					return ;
				}
				close(fd_out);
				args[i] = NULL;
				args[i + 1] = NULL;
				i++;
			}
			else
			{
				fprintf(stderr, "Error: missing filename after '%s'\n", args[i]);
				*exit_status = 1;
				return ;
			}
		}
		else if (strcmp(args[i], "<<") == 0)
		{
			if (args[i + 1])
			{
				char *delimiter = args[i + 1];
				int pipe_fd[2];
				if (pipe(pipe_fd) == -1)
				{
					perror("pipe");
					*exit_status = 1;
					return;
				}
				pid_t pid = fork();
				if (pid == 0)
				{
					close(pipe_fd[0]);
					char *line = NULL;
					size_t len = 0;
					while (1)
					{
						printf("> ");
						if (getline(&line, &len, stdin) == -1)
							break ;
						line[strcspn(line, "\n")] = '\0';
						if (strcmp(line, delimiter) == 0)
							break ;
						write(pipe_fd[1], line, strlen(line));
						write(pipe_fd[1], "\n", 1);
					}
					free(line);
					close(pipe_fd[1]);
					exit(0);
				}
				else if (pid > 0)
				{
					close(pipe_fd[1]);
					if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
					{
						perror("dup2");
						close(pipe_fd[0]);
						*exit_status = 1;
						return;
					}
					close(pipe_fd[0]);
					waitpid(pid, NULL, 0);
				}
				else
				{
					perror("fork");
					*exit_status = 1;
					return;
				}
				args[i] = NULL;
				args[i + 1] = NULL;
				i++;
			}
			else
			{
				fprintf(stderr, "Error: missing delimiter after '<<'\n");
				*exit_status = 1;
				return;
			}
		}
		i++;
	}
}
*/
