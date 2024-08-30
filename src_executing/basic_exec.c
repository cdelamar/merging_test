/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:24:07 by cdelamar          #+#    #+#             */
/*   Updated: 2024/08/06 21:14:34 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern volatile int g_var;

#include "../includes/executing.h"

int set_command_path(t_cmd *cmd)
{
    ft_path(cmd);
    if (!cmd->path)
    {
        printf("command not found\n");
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int basic_child_process(char **line_parsed, t_cmd *cmd)
{
    char *command;

    if (handle_redirections(line_parsed, HEREDOC_ON, cmd) != 0)
    {
        //printf("ERROR (basic_exec.c line 25)\n");
        return EXIT_FAILURE;
    }

    command = cmd_finder(line_parsed, cmd);
    if (command)
    {
        //printf ("execve ?\n");
        execve(command, line_parsed, cmd->env);
    }
    //printf("apres execve\n");
    printf("command not found: %s\n", line_parsed[0]);
    free_structs(cmd);
    return EXIT_FAILURE;
}

int basic_parent_process(pid_t pid, char **split_line) // TODO free cmd->path_split
{
    int status;

    if (waitpid(pid, &status, 0) == -1)
    {
        //printf("waitpid -1\n");
        if (split_line)
            ft_freetab(split_line);
        return EXIT_FAILURE;
    }
    if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE) // cest la que ca peche ft_peche
    {
        //printf("(line %d)\n", __LINE__);
        if (split_line)
          ft_freetab(split_line);
        return EXIT_FAILURE;
    }
    ft_freetab(split_line);
    return EXIT_SUCCESS;
}

int basic_execute(char **line_parsed, t_cmd *cmd)
{
    int exit_code;

    exit_code = set_command_path(cmd);
    if (exit_code != EXIT_SUCCESS)
    {
        ft_freetab(line_parsed);
        free_structs(cmd);
        return exit_code;
    }

    cmd->pid1 = fork();
    if (cmd->pid1 < 0)
    {
        printf("Error forking\n");
        return EXIT_FAILURE;
    }
    else if (cmd->pid1 == 0)
    {
        exit_code = basic_child_process(line_parsed, cmd);
        ft_freetab(line_parsed);
        exit(exit_code);
    }
    else
        return basic_parent_process(cmd->pid1, line_parsed);

    return EXIT_SUCCESS;
}
/*
int basic_execute(char **line_parsed, t_cmd *cmd)
{
    int exit_code;

    // Set command path from parsed input
    exit_code = set_command_path(cmd);
    if (exit_code != EXIT_SUCCESS)
    {
        printf("exit_code != EXIT_SUCCESS\n");
        free_structs(cmd);
        return exit_code;
    }

    cmd->pid1 = fork();
    if (cmd->pid1 < 0)
    {
        printf("Fork failed\n");
        return EXIT_FAILURE; // Error forking
    }
    else if (cmd->pid1 == 0)
    {
        exit_code = basic_child_process(line_parsed, cmd);
        exit(exit_code);
    }
    else
    {
        return basic_parent_process(cmd->pid1, line_parsed);
    }
    return EXIT_SUCCESS;
}*/
