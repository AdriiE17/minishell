/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:30:57 by victor            #+#    #+#             */
/*   Updated: 2024/12/03 11:40:31 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

int	main(int argc, char *argv[], char *env[])
{
	char	*line;
	char	prompt[PATH_MAX + HOST_NAME_MAX + 50];
	int		exit_status;

	exit_status = 0;
	if (!initialize_shell(argc, argv))
		return (0);
	while (1)
	{
		if (g_signal_received)
			g_signal_received = 0;
		line = handle_prompt(prompt, env);
		handle_exit(line, exit_status);
		process_line(line, &exit_status);
	}
	return (exit_status);
}
