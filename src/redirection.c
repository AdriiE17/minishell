/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:07:58 by vberdugo          #+#    #+#             */
/*   Updated: 2024/12/04 15:35:34 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	strip_quotes(char *str)
{
	int	len;

	len = ft_strlen(str);
	if ((str[0] == '\'' || str[0] == '"') && str[len - 1] == str[0])
	{
		ft_memmove(str, str + 1, len - 2);
		str[len - 2] = '\0';
	}
}

void	handle_redirections(char **args, int *exit_status)
{
	int		fd;
	int		i;
	int		append;
	char	*delimiter;
	int		pipefds[2];
	char	*line;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0)
		{
			append = ft_strcmp(args[i], ">>") == 0;
			strip_quotes(args[i + 1]);
			fd = open(args[i + 1], O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
			if (fd == -1)
			{
				perror(args[i + 1]);
				*exit_status = 1;
				return ;
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL;
		}
		else if (ft_strcmp(args[i], "<") == 0)
		{
			strip_quotes(args[i + 1]);
			fd = open(args[i + 1], O_RDONLY);
			if (fd == -1)
			{
				perror(args[i + 1]);
				*exit_status = 1;
				return ;
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			args[i] = NULL;
		}
		else if (ft_strcmp(args[i], "<<") == 0)
		{
			delimiter = args[i + 1];
			if (pipe(pipefds) == -1)
			{
				perror("pipe");
				*exit_status = 1;
				return ;
			}
			if (fork() == 0)
			{
				close(pipefds[0]);
				while ((line = readline("> ")) != NULL)
				{
					if (ft_strcmp(line, delimiter) == 0)
						break ;
					write(pipefds[1], line, ft_strlen(line));
					write(pipefds[1], "\n", 1);
					free(line);
				}
				close(pipefds[1]);
				exit(0);
			}
			close(pipefds[1]);
			dup2(pipefds[0], STDIN_FILENO);
			close(pipefds[0]);
			args[i] = NULL;
		}
		i++;
	}
}
