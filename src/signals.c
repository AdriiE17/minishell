/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:31:38 by vberdugo          #+#    #+#             */
/*   Updated: 2024/12/07 12:35:52 by victor           ###   ########.fr       */
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
		rl_redisplay();
	}
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
	signal(SIGQUIT, signal_handler);
	return (1);
}

/* ************************************************************************** */
/* Handles the shell's exit procedure. Frees the input line and terminates    */
/* the shell process with the provided exit status if the input is NULL       */
/* or matches the "exit" command.                                             */
/* ************************************************************************** */
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

/* ************************************************************************** */
/* Processes a single line of user input. Adds the line to the command        */
/* history, passes it to the command processor, and frees the allocated       */
/* memory for the line.                                                       */
/* ************************************************************************** */
void	process_line(char *line, int *exit_status)
{
	add_history(line);
	process_command(line, exit_status);
	free(line);
}
