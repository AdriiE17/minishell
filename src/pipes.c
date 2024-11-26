/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:38:32 by vberdugo          #+#    #+#             */
/*   Updated: 2024/11/26 18:37:13 by victor           ###   ########.fr       */
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

void execute_pipeline(char *cmd, int *exit_status)
{
	char *sub_token;
	int pipefds[2];
	int prev_pipefd = -1;
	pid_t pid;

	sub_token = strtok(cmd, "|");
	while (sub_token != NULL) {
		if (pipe(pipefds) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == 0) {
			if (prev_pipefd != -1) {
				dup2(prev_pipefd, STDIN_FILENO); // Leer del pipe previo
				close(prev_pipefd);
			}
			if (strtok(NULL, "|") != NULL) { // No es el último comando
				dup2(pipefds[1], STDOUT_FILENO); // Escribir al pipe actual
			}
			close(pipefds[0]);
			close(pipefds[1]);
			char **args = split_args(sub_token);
			if (ft_is_builtin(args[0])) {
				ft_execute(args[0], exit_status); // Ejecutar built-in
				exit(*exit_status); // Terminar proceso hijo
			} else {
				if (execvp(args[0], args) == -1) {
					perror("execvp");
					exit(EXIT_FAILURE);
				}
			}
		} else if (pid < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (prev_pipefd != -1) {
			close(prev_pipefd); // Cerrar el pipe anterior
		}
		close(pipefds[1]); // Cerrar el extremo de escritura del pipe actual
		prev_pipefd = pipefds[0]; // El extremo de lectura es ahora el pipe previo

		sub_token = strtok(NULL, "|");
	}
	if (prev_pipefd != -1)
		close(prev_pipefd);
	while (wait(NULL) > 0)
		;
}

