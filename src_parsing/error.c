/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:42:17 by laubry            #+#    #+#             */
/*   Updated: 2024/10/31 14:37:43 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe(char **split_line)
{
	int	i;
	int	j;

	i = 0;
	while (split_line[i])
	{
		j = 0;
		while (split_line[i][j])
		{
			// comme le tiens d'avant mais on check si [i + 1] == NULL ou pas
			if (split_line[i][j] == '|' && split_line[i + 1] && split_line[i + 1][j] == '|')
				return (0);
			j++;
		}
		// evite les segfault en cas de ' | ' qui se baladerait tout seul a la fin de la chaine
		if (split_line[i][0] == '|' && (split_line[i + 1] == NULL || split_line[i + 1][0] == '\0'))
			return (0);
		i++;
	}
	return (1);
}

int	check_error(int code_error)
{
	if (code_error == ERROR_NODE)
		printf("node creation error");
	else if (code_error == ERROR_ARGS)
		printf("error arguments");
	else if (code_error == ERROR_QUOTE)
		printf("error solo quote detected");
	return (0);
}

int	check_quote_error(char *line)
{
	int	tab[3];
	int	i;

	ft_bzero(tab, sizeof(tab));
	i = 0;
	while (line[i])
	{
		if (line[i] == '"')
		{
			if (!tab[1])
				tab[2] = !tab[2];
		}
		else if (line[i] == '\'')
		{
			if (!tab[2])
				tab[1] = !tab[1];
		}
		i++;
	}
	if (tab[2] || tab[1])
		return (1);
	return (check_error(ERROR_QUOTE));
}
