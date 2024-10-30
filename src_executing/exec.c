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

bool pipe_found(t_token *token_list)
{
    while (token_list != NULL)
    {
		//printf("content : %s\n", token_list->content);
        if (token_list->type == PIPE)
		{
			// printf("PIPE TRUE\n");
            return (true);
		}
		token_list = token_list->next;
    }
	// printf("PIPE FALSE\n");
    return (false);
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
	if (pipe_found(cmd->tokens) == true)
	{
	//	printf("into PIPE\n");
		return (pipe_execute(cmd));
	}
	/*else if (ft_builtin(cmd) == EXIT_SUCCESS)
	{
		//printf("into builtins");
		return (EXIT_SUCCESS);
	}*/
	else
	{
	//	printf("into BASIC");
		return (basic_execute(cmd));
	}
}