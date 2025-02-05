/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_validate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:45:43 by vberdugo          #+#    #+#             */
/*   Updated: 2025/02/05 17:59:38 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_syntax(const char *cmd, int *ex)
{
	int	i;
	int	len;

	if (!cmd)
		return (1);
	len = ft_strlen(cmd);
	i = -1;
	while (cmd[++i])
	{
		*ex = 2;
		if (cmd[i] == '|' && cmd[i + 1] == '|')
		{
			printf("syntax error near unexpected token `||'\n");
			return (1);
		}
	}
	if (cmd[0] == '|')
	{
		*ex = 2;
		printf("syntax error near unexpected token `|'\n");
		return (1);
	}
	if (cmd[len - 1] == '|')
		return (*ex = 2, printf("syntax error near unexpected token `|'\n"), 1);
	return (0);
}

bool	has_unclosed_quotes(const char *line)
{
	bool	in_single;
	bool	in_double;

	in_single = false;
	in_double = false;
	while (*line)
	{
		if (*line == '\'' && !in_double)
			in_single = !in_single;
		else if (*line == '"' && !in_single)
			in_double = !in_double;
		line++;
	}
	return (in_single || in_double);
}

char	*handle_multiline_input(char *initial_line)
{
	char	*joined_line;
	char	*new_line;

	joined_line = ft_strdup(initial_line);
	while (has_unclosed_quotes(joined_line))
	{
		free(initial_line);
		initial_line = readline("> ");
		if (!initial_line)
			return (free(joined_line), NULL);
		new_line = malloc(ft_strlen(joined_line) + ft_strlen(initial_line) + 2);
		if (!new_line)
		{
			free(joined_line);
			free(initial_line);
			return (NULL);
		}
		ft_strcpy(new_line, joined_line);
		ft_strcat(new_line, "\n");
		ft_strcat(new_line, initial_line);
		free(joined_line);
		joined_line = new_line;
	}
	return (free(initial_line), joined_line);
}

char	**split_args(const char *cmd)
{
	t_expan	exp;
	char	*token;
	char	**tm;

	exp = init_expan(cmd);
	if (!exp.cmd_copy)
		return (NULL);
	exp.arg = NULL;
	token = ft_strsep(&exp.ptr, " ", &exp.in_single, &exp.quote_char);
	while (token != NULL)
	{
		if (*token != '\0')
		{
			tm = ft_realloc(exp.arg, S_PTR * exp.ind, S_PTR * (exp.ind + 1));
			if (!tm)
				return (free(exp.cmd_copy), free(exp.arg), NULL);
			exp.arg = tm;
			exp.arg[exp.ind++] = ft_strdup(token);
		}
		token = ft_strsep(&exp.ptr, " ", &exp.in_single, &exp.quote_char);
	}
	tm = ft_realloc(exp.arg, S_PTR * exp.ind, S_PTR * (exp.ind + 1));
	if (!tm)
		return (free(exp.cmd_copy), NULL);
	return (exp.arg = tm, exp.arg[exp.ind] = NULL, free(exp.cmd_copy), exp.arg);
}
