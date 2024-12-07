/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 19:11:28 by victor            #+#    #+#             */
/*   Updated: 2024/12/07 12:17:24 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/* Removes single quotes from a string. It iterates through the input string  */
/* and skips over any single quotes that are not preceded by a backslash.     */
/* The function modifies the original string in place by shifting the         */
/* characters to remove the quotes.                                           */
/* ************************************************************************** */
void	strip_single_quotes(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' && (i == 0 || str[i - 1] != '\\'))
		{
			i++;
			continue ;
		}
		str[j++] = str[i++];
	}
	str[j] = '\0';
}

/* ************************************************************************** */
/* Removes double quotes from a string. It processes the input string by      */
/* skipping over double quotes unless they are escaped with a backslash.      */
/* It also handles the special case of removing the '$' character only when   */
/* it is not within double quotes. The function modifies the original string  */
/* in place by shifting the characters to remove the quotes and the dollar    */
/* signs.                                                                     */
/* ************************************************************************** */
void	strip_double_quotes(char *str)
{
	int	i;
	int	j;
	int	in_dollar;

	i = 0;
	j = 0;
	in_dollar = 0;
	while (str[i])
	{
		if (str[i] == '"' && (i == 0 || str[i - 1] != '\\'))
		{
			i++;
			continue ;
		}
		if (str[i] == '$' && !in_dollar)
			in_dollar = 1;
		else if (str[i] != '$' && in_dollar)
			in_dollar = 0;
		if (in_dollar || (str[i] != '$'))
			str[j++] = str[i++];
		else
			i++;
	}
	str[j] = '\0';
}

/* ************************************************************************** */
/* Removes surrounding quotes from a string, if present. It checks if the     */
/* first and last characters are matching quotes (either single or double).   */
/* If they are, it removes the quotes and adjusts the string accordingly.     */
/* The function modifies the original string in place.                        */
/* ************************************************************************** */
void	strip_quotes(char *str)
{
	int	len;

	len = ft_strlen(str);
	if ((str[0] == '\'' || str[0] == '"') && str[len - 1] == str[0])
	{
		ft_memmove(str, str + 1, len - 2);
		str[len - 2] = '\0';
	}
}
