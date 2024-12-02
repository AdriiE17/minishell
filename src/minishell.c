/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:30:57 by victor            #+#    #+#             */
/*   Updated: 2024/12/02 19:53:22 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

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
		else
		{
			printf("Ctrl+C detectado ejecutando un comando bloqueante\n");
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

int	main(int argc, char *argv[], char *env[])
{
	char	*line;
	char	prompt[PATH_MAX + HOST_NAME_MAX + 50];
	int		exit_status;

	exit_status = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	configure_terminal();
	if (!print_version(argc, argv))
		return (0);
	while (1)
	{
		if (g_signal_received)
		{
			g_signal_received = 0;
		}
		line = handle_prompt(prompt, env);
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		add_history(line);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			printf("exit\n");
			break ;
		}
		process_command(line, &exit_status);
		free(line);
	}
	return (exit_status);
}
