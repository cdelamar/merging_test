/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:32:50 by cdelamar          #+#    #+#             */
/*   Updated: 2024/08/08 13:48:49 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/executing.h"

static bool contains_pipe(char **line_parsed)
{
    int i = 1;

    while (line_parsed[i])
    {
        if (ft_strcmp(line_parsed[i], "|") == 0)
            return true;
        i++;
    }
    return false;
}

void cleanup(char *line, t_cmd *cmd)
{
    free_structs(cmd);
    free(line);
}

void initialize_cmd(t_cmd *cmd, char **line_parsed)
{
    cmd->fd_in = 0; // Initialize the input for the first command
    cmd->path_command = line_parsed; // Direct assignment since it's already an array of commands
}

/*
void initialize_cmd(t_cmd *cmd, char *line)
{
    cmd->fd_in = 0; // Initialize the input for the first commad
    cmd->path_command = ft_split(line, '|'); // maybe the issue of '|' = ctrl + d.
}*/

void handle_error(char *msg, t_cmd *cmd, int *fd)
{
    printf("ERROR (%s)\n", msg);
    close_fds(fd);
	if (cmd->path_command)
        ft_freetab(cmd->path_command);
    //return(EXIT_FAILURE);
	exit(EXIT_FAILURE);
}

int	execute(char **line_parsed, t_cmd *cmd)
{
	if (line_parsed == NULL || line_parsed[0] == NULL)
		return (0);

	// Syntax error check for pipe at the start
	if (ft_strcmp(line_parsed[0], "|") == 0)
	{
		printf("line parsed >> syntax error: expected arguments with '|'\n");
		return (0);
	}

	// Handle pipeline commands
	if (contains_pipe(line_parsed) == true)
		return (pipe_execute(line_parsed, cmd));

	// Handle built-in commands
	if (ft_builtin(line_parsed, cmd) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);

	// Handle external commands
	return (basic_execute(line_parsed, cmd));
}


// int	execute(char *line, char **line_parsed, t_cmd *cmd)
// {
// 	// printf ("** execute **\n");
// 	if (ft_strcmp(line_parsed[0], "|") == 0)
// 	{
// 		printf("line parsed >> synthax error : expected arguments with '|'\n");
// 		return (0);
// 	}

// 	if (line[0] == '\0')
// 	{
// 		//printf("free : nothing outputed (exec.c : line 43)\n");
// 		return (0);
// 	}

// 	//if (handle_exit_command(line) == EXIT_COMMAND)
// 	//	return (EXIT_COMMAND);
// 	if (ft_strchr(line, '|'))
// 		return (pipe_execute(line, cmd));
// 	else if (ft_builtin(line, cmd) == EXIT_SUCCESS)
// 	{
// 		//printf("builtins succeed (exec.c line 52)\n");
// 		return (EXIT_SUCCESS);
// 	}
// 	/*else if (ft_strcmp(line, "exit") == 0)
// 		return (EXIT_COMMAND);*/
// 	return (basic_execute(line, cmd));
// }
