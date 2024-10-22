/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:32:50 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/22 15:59:01 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_error(char *msg, t_cmd *cmd, int *fd)
{
	printf("ERROR (%s)\n", msg);
	close_fds(fd);
	if (cmd->path_command)
		ft_freetab(cmd->path_command);
	exit(EXIT_FAILURE);
}

int	execute(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (ft_strcmp(cmd->final_line, "|") == 0)
	{
		printf("synthax error : expected arguments with '|'\n");
		return (0);
	}
	if (syntax_redirect(cmd->final_line) == false)
		return (0);
	if (cmd->final_line[0] == '\0')
		return (0);
	while (cmd->final_tab[i])
	{
		if (space_only(cmd->final_tab[i]))
		{
			printf("%s : command not found\n", cmd->final_tab[i]);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	if (ft_strchr(cmd->final_line, '|'))
		return (pipe_execute(cmd));
	else if (ft_builtin(cmd) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	else
		return (basic_execute(cmd));
}
