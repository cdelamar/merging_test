/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:13:47 by laubry            #+#    #+#             */
/*   Updated: 2024/10/22 16:22:29 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_commands(t_cmd *cmd)
{
	int	i;
	int	count;

	count = 1;
	i = 0;
	while (cmd->final_tab[i])
	{
		if (strcmp(cmd->final_tab[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
}

char	**split_commands(t_cmd *cmd)
{
	int		i;
	int		j;
	char	**commands;
	char	*temp;
	char	*tmp;

	i = 0;
	j = 0;
	commands = malloc((count_commands(cmd) + 1) * sizeof(char *));
	temp = NULL;
	tmp = NULL;
	while (cmd->final_tab[i])
	{
		if (strcmp(cmd->final_tab[i], "|") == 0)
		{
			commands[j++] = temp;
			temp = NULL;
		}
		else
		{
			if (!temp)
				temp = ft_strdup(cmd->final_tab[i]);
			else
			{
				tmp = malloc(strlen(temp) + ft_strlen(cmd->final_tab[i]) + 2);
				strcpy(tmp, temp);
				strcat(tmp, " ");
				strcat(tmp, cmd->final_tab[i]);
				free(temp);
				temp = tmp;
			}
		}
		i++;
	}
	commands[j++] = temp;
	commands[j] = NULL;
	return (commands);
}

void	ft_path_command(t_cmd *cmd)
{
	cmd->fd_in = 0;
	cmd->path_command = split_commands(cmd);
}
