/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:31:38 by vberdugo          #+#    #+#             */
/*   Updated: 2025/01/09 11:00:37 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* Handles terminal signals like SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\).        */
/* SIGINT: Displays a new prompt line and updates the global signal flag.     */
/* SIGQUIT: Clears the current line and redisplays the prompt.                */
/* ************************************************************************** */
void	signal_handler(int sig)
{
	if (sig == SIGQUIT)
	{
		if (!isatty(STDIN_FILENO))
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
	else if (sig == SIGINT)
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
	signal(SIGQUIT, SIG_IGN);
}

/* ************************************************************************** */
/* Configures the terminal settings to disable echoing of control characters  */
/* like ^C and ^\, ensuring a cleaner shell interface.                        */
/* ************************************************************************** */
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

/* ************************************************************************** */
/* Initializes the shell by checking the input arguments, configuring the     */
/* terminal, and setting up signal handlers for SIGINT and SIGQUIT.           */
/* Returns 1 on success, 0 otherwise.                                         */
/* ************************************************************************** */
int	initialize_shell(int argc, char *argv[])
{
	if (!print_version(argc, argv))
		return (0);
	configure_terminal();
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (1);
}

/* ************************************************************************** */
/* Handles the shell's exit procedure. Frees the input line and terminates    */
/* the shell process with the provided exit status if the input is NULL       */
/* or matches the "exit" command.                                             */
/* ************************************************************************** */
int	handle_exit(char *line, int exit_status)
{
	char	*arg;
	int		new_status;

	if (line == NULL || strncmp(line, "exit", 4) != 0)
		return (exit_status);
	arg = line + 4;
	while (*arg == ' ')
		arg++;
	if (*arg != '\0')
	{
		new_status = ft_atoi(arg);
		if (new_status == 0 && strcmp(arg, "0") != 0)
			return (exit_status);
		exit(new_status);
	}
	exit(exit_status);
}

/* ************************************************************************** */
/* Processes a single line of user input. Adds the line to the command        */
/* history, passes it to the command processor, and frees the allocated       */
/* memory for the line.                                                       */
/* ************************************************************************** */
void	process_line(char *line, int *exit_status, char **env)
{
	add_history(line);
	process_command(line, exit_status, env);
	free(line);
}
