/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_util2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:45:43 by vberdugo          #+#    #+#             */
/*   Updated: 2024/12/16 17:28:43 by vberdugo         ###   ########.fr       */
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
