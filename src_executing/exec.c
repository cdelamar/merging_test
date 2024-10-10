/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:32:50 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/03 02:11:27 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void cleanup(char *line, t_cmd *cmd)
{
	if(cmd)
    	free_structs(cmd);
    if(line)
		free(line);
}

void handle_error(char *msg, t_cmd *cmd, int *fd)
{
    printf("ERROR (%s)\n", msg);
    close_fds(fd);
	if (cmd->path_command)
        ft_freetab(cmd->path_command);
	exit(EXIT_FAILURE);
}

int	execute(t_cmd *cmd)
{
	if (ft_strcmp(cmd->final_line, "|") == 0)
	{
		printf("synthax error : expected arguments with '|'\n");
		return (0);
	}

	if (syntax_redirect(cmd->final_line) == false)
		return (0);

	if (cmd->final_line[0] == '\0')
		return (0);

	if (ft_strchr(cmd->final_line, '|'))
		return (pipe_execute(cmd));
	else if (ft_builtin(cmd) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	else
	{
		//printf("basic through exec\n\n");
		// ca leak en cas de commande invalide, pourquoi pas liberer ici jsp
		return (basic_execute(cmd, BASIC_EXEC, 0)); // EXIT_SUCCESS OR EXIT_FAILURE
	}
}
