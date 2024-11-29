/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 16:19:43 by victor            #+#    #+#             */
/*   Updated: 2024/11/27 17:34:57 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsep(char **stringp, const char *delim)
{
	char		*start;
	char		*end;
	const char	*d;

	start = *stringp;
	if (*stringp == NULL)
		return (NULL);
	end = *stringp;
	while (*end)
	{
		d = delim;
		while (*d)
		{
			if (*end == *d)
			{
				*end = '\0';
				*stringp = end + 1;
				return (start);
			}
			d++;
		}
		end++;
	}
	*stringp = NULL;
	return (start);
}
