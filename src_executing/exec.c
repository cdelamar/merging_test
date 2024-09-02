/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:32:50 by cdelamar          #+#    #+#             */
/*   Updated: 2024/09/02 17:56:34 by cdelamar         ###   ########.fr       */
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

// void initialize_cmd(t_cmd *cmd, char **line_parsed)
// {
//     cmd->fd_in = 0; // Initialize the input for the first command
//     cmd->path_command = line_parsed; // Direct assignment since it's already an array of commands
// }

void initialize_cmd(t_cmd *cmd, char *line) {
    cmd->fd_in = 0; // Set initial input
    cmd->path_command = ft_split(line, '|'); // Split line by pipe '|'
}

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
    printf("avant ft_builtin\n");
    print_tab(line_parsed);
	if (ft_builtin(line_parsed, cmd) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);

	// Handle external commands
	return (basic_execute(line_parsed, cmd));
}

/*int execute(char **line_parsed, t_cmd *cmd)
{
    if (line_parsed == NULL || line_parsed[0] == NULL)
        return 0;

    if (ft_strcmp(line_parsed[0], "|") == 0)
    {
        printf("Syntax error: expected arguments with '|'\n");
        return 0;
    }

    if (contains_pipe(line_parsed))
        return pipe_execute(line_parsed, cmd);

    if (ft_builtin(line_parsed, cmd) == EXIT_SUCCESS)
        return EXIT_SUCCESS;

    return basic_execute(line_parsed, cmd);
}*/
