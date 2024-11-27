/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:56:07 by victor            #+#    #+#             */
/*   Updated: 2024/11/27 13:57:39 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(char *str)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (*str == '|')
		{
			count++;
		}
		str++;
	}
	return (count + 1);
}

void	handle_pipe_error(void)
{
	perror("pipe");
	exit(EXIT_FAILURE);
}

void	handle_fork_error(void)
{
	perror("fork");
	exit(EXIT_FAILURE);
}

void	close_pipe(int pipefd)
{
	if (pipefd != -1)
	{
		close(pipefd);
	}
}

void	wait_for_children(void)
{
	while (wait(NULL) > 0)
		;
}
