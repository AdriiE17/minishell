/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:18:35 by victor            #+#    #+#             */
/*   Updated: 2024/12/18 10:11:45 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* Expands the exit status variable into the input string. Converts the       */
/* integer exit_status to a string and copies it into the expanded string     */
/* at the correct position. It returns the updated index in the expanded      */
/* string.                                                                    */
/* ************************************************************************** */
static int	expand_exit_status(char *expanded, int i, int exit_status)
{
	char	*exit_str;
	int		j;

	exit_str = ft_itoa(exit_status);
	j = 0;
	while (exit_str[j])
		expanded[i++] = exit_str[j++];
	free(exit_str);
	return (i);
}

/* ************************************************************************** */
/* Expands an environment variable found in the input string. It extracts     */
/* the variable name, looks up its value using getenv(), and copies the       */
/* value into the expanded string at the correct position. The function       */
/* returns the updated index in the expanded string.                          */
/* ************************************************************************** */
static int	exp_single_env_var(char *expanded, char **ptr, int i)
{
	char	var_name[1024];
	int		j;
	char	*env_value;

	j = 0;
	while (**ptr && (ft_isalnum(**ptr) || **ptr == '_'))
	{
		var_name[j++] = **ptr;
		(*ptr)++;
	}
	var_name[j] = '\0';
	env_value = getenv(var_name);
	if (env_value)
	{
		while (*env_value)
			expanded[i++] = *env_value++;
	}
	return (i);
}

/* ************************************************************************** */
/* Handles special characters like single quotes, double quotes, and the      */
/* dollar sign in the input string. It toggles single or double quotes and    */
/* expands environment variables or the exit status if necessary. The         */
/* function returns 1 if a special character was processed, and 0 if not.     */
/* ************************************************************************** */
int	handle_special_chars2(t_expan *exp, char **input, int exit)
{
	if (**input == '\'' && !exp->in_double)
	{
		exp->in_single = !exp->in_single;
		(*input)++;
		return (1);
	}
	if (**input == '"' && !exp->in_single)
	{
		exp->in_double = !exp->in_double;
		(*input)++;
		return (1);
	}
	if (**input == '$' && !exp->in_single)
	{
		(*input)++;
		if (**input == '?')
		{
			(*input)++;
			exp->ind = expand_exit_status(exp->expanded, exp->ind, exit);
		}
		else
			exp->ind = exp_single_env_var(exp->expanded, input, exp->ind);
		return (1);
	}
	return (0);
}

static int	handle_special_chars(t_expan *exp, char **input, int exit)
{
	if (*(*input) == '\'' || *(*input) == '"')
	{
		if (*(*input) == '\'' && !exp->in_double)
			exp->in_single = !exp->in_single;
		else if (*(*input) == '"' && !exp->in_single)
			exp->in_double = !exp->in_single;
		exp->expanded[exp->ind++] = *(*input)++;
		return (1);
	}
	if (handle_special_chars2(exp, input, exit))
		return (1);
	return (0);
}

/* ************************************************************************** */
/* Expands environment variables within the input string. It handles special  */
/* characters, such as quotes and the dollar sign ('$'), expanding them into  */
/* the corresponding values (e.g., environment variables or exit status).     */
/* The function returns a new string with the expanded variables.             */
/* ************************************************************************** */
char	*expand_all_env_vars(char *input, int exit_status)
{
	int			cap;
	t_expan		exp;

	cap = 1024;
	exp.expanded = malloc(cap);
	if (!exp.expanded)
		return (NULL);
	exp.ptr = input;
	exp.ind = 0;
	exp.in_single = 0;
	exp.in_double = 0;
	while (*exp.ptr)
	{
		if (handle_special_chars(&exp, &exp.ptr, exit_status))
			continue ;
		if (exp.ind >= cap - 1)
		{
			cap *= 2;
			exp.expanded = ft_realloc(exp.expanded, cap / 2, cap);
			if (!exp.expanded)
				return (NULL);
		}
		exp.expanded[exp.ind++] = *exp.ptr++;
	}
	return (exp.expanded[exp.ind] = '\0', exp.expanded);
}
