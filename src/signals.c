/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:31:38 by vberdugo          #+#    #+#             */
/*   Updated: 2024/12/03 11:33:46 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		if (isatty(STDIN_FILENO))
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		g_signal_received = SIGINT;
	}
	else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	configure_terminal(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

int	initialize_shell(int argc, char *argv[])
{
	if (!print_version(argc, argv))
		return (0);
	configure_terminal();
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	return (1);
}

int	handle_exit(char *line, int exit_status)
{
	if (line == NULL || ft_strcmp(line, "exit") == 0)
	{
		printf("exit\n");
		free(line);
		exit(exit_status);
	}
	return (0);
}

void	process_line(char *line, int *exit_status)
{
	add_history(line);
	process_command(line, exit_status);
	free(line);
}
