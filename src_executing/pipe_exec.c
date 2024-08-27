/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:23:45 by cdelamar          #+#    #+#             */
/*   Updated: 2024/08/08 13:31:31 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/executing.h"


// TODO : fix   MINISHELL>qwwqls | wc
//              1       4      27

static void setup_child_pipes(t_cmd *cmd, int *fd, int i)
{
    dup2(cmd->fd_in, 0); // Get input from the previous command
    if (cmd->path_command[i + 1])
        dup2(fd[1], 1); // Output to the next command
    close_fds(fd);
    close(cmd->fd_in);
}

static int child_process(t_cmd *cmd, int *fd, int i)
{
    setup_child_pipes(cmd, fd, i);

    // Split the individual command at the current index `i`
    char **split_line = ft_split(cmd->path_command[i], ' ');

    if (ft_strcmp(split_line[0], "exit") == 0)
    {
        int exit_code = ft_exit(split_line, cmd);
        ft_freetab(split_line);
        exit(exit_code);
    }

    // If not 'exit', execute normally
    if (basic_execute(split_line, cmd) == EXIT_FAILURE)
    {
        ft_freetab(split_line);
        exit(EXIT_FAILURE);
    }
    ft_freetab(split_line);
    exit(EXIT_SUCCESS);
}

static void parent_process(t_cmd *cmd, int *fd, int *i)
{
    int	status;

    waitpid(cmd->pid1, &status, 0);
    close(fd[1]);
    if (cmd->fd_in)
        close(cmd->fd_in);
    cmd->fd_in = fd[0]; // save input for nxt command
    (*i)++;				// (indexing increment)
}

static int create_and_fork(t_cmd *cmd, int *fd)
{
    if (pipe(fd) < 0)
        handle_error("exec.c line 49", cmd, fd);
    cmd->pid1 = fork();
    if (cmd->pid1 < 0)
        handle_error("exec.c line 57", cmd, fd);
    return cmd->pid1;
}


int pipe_execute(char **line_parsed, t_cmd *cmd)
{
    int i = 0;

    // Initialize command using the parsed lines
    initialize_cmd(cmd, line_parsed);

    while (cmd->path_command[i])
    {
        //printf("** cmd->path_command[%d] = %s **\n", i, cmd->path_command[i]);
        if (create_and_fork(cmd, cmd->fd) == 0)
        {
            //printf("into child_process\n");
            child_process(cmd, cmd->fd, i);
        }
        else
        {
            //printf("into parent_process\n");
            parent_process(cmd, cmd->fd, &i);
        }
        i++; // This was previously incremented twice; now it should only be incremented here
    }

    close(cmd->fd_in);
    if (cmd->path_command)
        ft_freetab(cmd->path_command);
    return (EXIT_SUCCESS);
}
/*int pipe_execute(char **line_parsed, t_cmd *cmd)
{
    int i = 0;

    initialize_cmd(cmd, line_parsed);

    while (cmd->path_command[i])
    {
        if (create_and_fork(cmd, cmd->fd) == 0)
            child_process(cmd, cmd->fd, i);
        else
            parent_process(cmd, cmd->fd, &i);
    }

    close(cmd->fd_in);
    if (cmd->path_command)
        ft_freetab(cmd->path_command);
    return (EXIT_SUCCESS);
}*/

