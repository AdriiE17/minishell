/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 11:04:56 by victor            #+#    #+#             */
/*   Updated: 2024/12/15 11:05:53 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?' || c == '$');
}

void	swap_word(char *word, char **s, char *init, int i)
{
	int		k;
	char	tmp[900];

	if (!strcmp("$", word) && init[1])
		word++;
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

void	remove_q(char **s)
{
	int		i;
	int		j;
	bool	in_single_quote;
	bool	in_double_quote;
	char	tmp[900];

	i = 0;
	j = 0;
	in_single_quote = false;
	in_double_quote = false;
	while ((*s)[i])
	{
		if ((*s)[i] == S_QUOTE && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if ((*s)[i] == D_QUOTE && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			tmp[j++] = (*s)[i];
		i++;
	}
	tmp[j] = '\0';
	free(*s);
	*s = strdup(tmp);
}

void	handle_q(char **s)
{
	int		i;
	char	*word;

	i = 0;
	word = NULL;
	while ((*s)[i])
	{
		if ((*s)[i] == S_QUOTE || (*s)[i] == D_QUOTE)
		{
			if (word != NULL)
			{
				swap_word(word, s, (*s) + i, -1);
				free(word);
				word = NULL;
			}
		}
		i++;
	}
}

void	process_string(char **s)
{
	handle_q(s);
	remove_q(s);
}
