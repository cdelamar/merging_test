/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:27:38 by laubry            #+#    #+#             */
/*   Updated: 2024/10/22 17:28:26 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

int	nbr_of_strs(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}

void	print_split(char **split_line)
{
	int	i;

	i = 0;
	while (split_line[i])
	{
		printf("print_split :%s\n", split_line[i]);
		i++;
	}
}

int	is_qte(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

void	print_free_tab(char **final_token)
{
	int	i;

	i = 0;
	while (final_token[i])
	{
		printf("tab == %s\n", final_token[i]);
		free(final_token[i]);
		i++;
	}
	free(final_token);
}
