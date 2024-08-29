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


static bool pipe_synthax(char **line_parsed)
{
    int i;

    i = 0;

    // Check if the first token is a pipe or empty
    if (line_parsed[i][0] == '|' || line_parsed[i][0] == '\0')
    {
        printf("Syntax error: command cannot start with '|'\n");
        return (false);
    }

    // Check the rest of the tokens
    while (line_parsed[i])
    {
        // Check if the current token is a pipe
        if (ft_strcmp(line_parsed[i], "|") == 0)
        {
            // If the next token is NULL or another pipe, it's an error
            if (!line_parsed[i + 1] || ft_strcmp(line_parsed[i + 1], "|") == 0)
            {
                printf("Syntax error: unexpected '|'\n");
                return (false);
            }
        }
        else if (line_parsed[i][0] == '|')
        {
            // Check for invalid cases like '|i'
            if (line_parsed[i][1] != '\0')
            {
                printf("Syntax error: invalid pipe usage '%s'\n", line_parsed[i]);
                return (false);
            }
        }
        i++;
    }

    // Check if the last token is a pipe
    if (ft_strcmp(line_parsed[i - 1], "|") == 0)
    {
        printf("Syntax error: command cannot end with '|'\n");
        return (false);
    }

    return (true);
}

int pipe_execute(char **line_parsed, t_cmd *cmd)
{
    int i = 0;

    if (pipe_synthax(line_parsed) == false)
    {
        //free
        printf ("synthax error with symbol '|'\n");
        return (EXIT_SUCCESS);
    }

    // Initialize command using the parsed lines
    initialize_cmd(cmd, cmd->saved_line);

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
        //i++; // This was previously incremented twice; now it should only be incremented here
    }

    close(cmd->fd_in);
    if (cmd->path_command)
        ft_freetab(cmd->path_command);
    return (EXIT_SUCCESS);
}

