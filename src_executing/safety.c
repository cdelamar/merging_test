/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safety.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:48:35 by laubry            #+#    #+#             */
/*   Updated: 2024/10/22 16:49:54 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	space_only(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (false);
		i++;
	}
	return (true);
}

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("array %d : %s\n", i, tab[i]);
		i++;
	}
}

bool	freeable_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
		i++;
	if (i <= 1)
		return (false);
	return (true);
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd->path_command)
		ft_freetab(cmd->path_command);
	if (cmd->path_split)
		ft_freetab(cmd->path_split);
	if (cmd->final_tab)
		ft_freetab(cmd->final_tab);
	free(cmd);
}
