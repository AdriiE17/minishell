/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 12:07:58 by vberdugo          #+#    #+#             */
/*   Updated: 2024/12/15 00:37:30 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
char	*remove_quotes(const char *str)
{
	size_t	len;
	size_t	start;
	size_t	end;
	size_t	i;
	char	*result;

	len = ft_strlen(str);
	start = 0;
	end = len - 1;
	while (str[start] == '"' || str[start] == '\'')
		start++;
	while (str[end] == '"' || str[end] == '\'')
		end--;
	if (start > end)
		return (ft_strdup(""));
	result = malloc(end - start + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (start <= end)
		result[i++] = str[start++];
	result[i] = '\0';
	return (result);
}*/
#include <stdbool.h>

#define D_QUOTE '"'
#define S_QUOTE '\''

bool is_valid_char(char c)
{
	return (isalnum(c) || c == '_' || c == '?' || c == '$');
}

void swap_word(char *word, char **s, char *init, int i)
{
	int k;
	char tmp[900];
	if (!strcmp("$", word) && init[1]) {
		word++;
	}
	while ((*s)[++i] != *init)
		tmp[i] = (*s)[i];
	k = -1;
	while (word && word[++k])
	{
		if (word[k] == ' ')
			tmp[i++] = '^';
		else
			tmp[i++] = word[k];
	}
	init++;
	while (*init && is_valid_char(*init))
		init++;
	while (*init)
		tmp[i++] = *init++;
	tmp[i] = '\0';
	free(*s);
	*s = strdup(tmp);
}

void remove_q(char **s)
{
	int i = 0, j = 0;
	bool in_single_quote = false, in_double_quote = false;
	char tmp[900];

	while ((*s)[i]) {
		if ((*s)[i] == S_QUOTE && !in_double_quote) {
			in_single_quote = !in_single_quote;
		} else if ((*s)[i] == D_QUOTE && !in_single_quote) {
			in_double_quote = !in_double_quote;
		} else {
			tmp[j++] = (*s)[i];
		}
		i++;
	}
	tmp[j] = '\0';
	free(*s);
	*s = strdup(tmp);
}

void handle_q(char **s)
{
	int i = 0;
	char *word = NULL;
	while ((*s)[i]) {
		if ((*s)[i] == S_QUOTE || (*s)[i] == D_QUOTE) {
			if (word != NULL) {
				swap_word(word, s, (*s) + i, -1);
				free(word);
				word = NULL;
			}
		}
		i++;
	}
}

void process_string(char **s) {
	handle_q(s);
	remove_q(s);
}

char *remove_quotes(char *str) {
	process_string(&str);
	return str;
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
