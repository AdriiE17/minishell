/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:07:58 by vberdugo          #+#    #+#             */
/*   Updated: 2024/12/15 11:08:51 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *str)
{
	process_string(&str);
	return (str);
}

/* ************************************************************************** */
/* Handles input redirection ('<') by checking if the next argument is a file */
/* and calling the corresponding function to perform the redirection. If the  */
/* filename is missing, it prints an error message and sets the exit status.  */
/* ************************************************************************** */
void	handle_input_redirection(char **args, int *exit_status, int *i)
{
	if (args[*i + 1])
		*exit_status = handle_input_redirect(args, *exit_status, i);
	else
	{
		printf("Error: missing filename after '<'\n");
		*exit_status = 1;
	}
}

/* ************************************************************************** */
/* Handles output redirection ('>' or '>>') by checking if the next argument  */
/* is a file and calling the corresponding function to perform the redirection*/
/* If the filename is missing, it prints an error message and sets the exit   */
/* status.                                                                    */
/* ************************************************************************** */
void	handle_output_redirection(char **args, int *exit_status, int *i)
{
	if (args[*i + 1])
		*exit_status = handle_output_redirect(args, *exit_status, i);
	else
	{
		printf("Error: missing file after '%s'\n", args[*i]);
		*exit_status = 1;
	}
}

/* ************************************************************************** */
/* Handles here document redirection ('<<') by checking if the next argument  */
/* is a delimiter and calling the corresponding function to perform the redir-*/
/* ection. If the delimiter is missing, it prints an error message and sets   */
/* the exit status.                                                           */
/* ************************************************************************** */
void	handle_here_doc_redirection(char **args, int *exit_status, int *i)
{
	if (args[*i + 1])
		*exit_status = handle_here_doc_redirect(args, *exit_status, i);
	else
	{
		printf("Error: missing delimiter after '<<'\n");
		*exit_status = 1;
	}
}

/* ************************************************************************** */
/* Loops through the arguments and checks for redirection operators ('<',     */
/* '>', '>>', '<<'). For each type of redirection operator, it calls the      */
/* corresponding handler function (handle_input_redirection, handle_output_   */
/* redirection, or handle_here_doc_redirection). It increments the index to   */
/* process all arguments.                                                     */
/* ************************************************************************** */
void	handle_redirections(char **args, int *exit_status)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "<") == 0)
			handle_input_redirection(args, exit_status, &i);
		else if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0)
			handle_output_redirection(args, exit_status, &i);
		else if (ft_strcmp(args[i], "<<") == 0)
			handle_here_doc_redirection(args, exit_status, &i);
		i++;
	}
}
