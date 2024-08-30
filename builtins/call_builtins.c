/* ************************************************************************** */


/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:27:10 by cdelamar          #+#    #+#             */
/*   Updated: 2024/07/25 10:36:34 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/executing.h"

static int builtin_commands(char **line_parsed, t_cmd *cmd, int saved_in, int saved_out)
{
    int ret;

	ret = EXIT_FAILURE;
    if (ft_strcmp(line_parsed[0], "unset") == 0)
        ret = ft_unset(line_parsed, cmd);
    else if (ft_strcmp(line_parsed[0], "echo") == 0)
        ret = ft_echo(line_parsed);
    else if (ft_strcmp(line_parsed[0], "cd") == 0)
        ret = ft_cd(line_parsed[1]);
    else if (ft_strcmp(line_parsed[0], "export") == 0)
        ret = ft_export(line_parsed, cmd);
    else if (ft_strcmp(line_parsed[0], "env") == 0)
        ret = ft_env(cmd);
    else if (ft_strcmp(line_parsed[0], "pwd") == 0)
        ret = ft_pwd();
    else if (ft_strcmp(line_parsed[0], "exit") == 0)
    {
        int exit_code = ft_exit(line_parsed, cmd);

        // If child process, directly exit
        if (cmd->pid1 == 0)
            exit(exit_code);

        // If parent process, return the exit code
        ret = exit_code;
    }

    restore_fd(saved_in, saved_out);
    return (ret);
}

static int redirect_manager(char **split_line, int saved_stdin, int saved_stdout, t_cmd *cmd)
{
    if (handle_redirections(split_line, 0, cmd) == EXIT_FAILURE)
	{
        restore_fd(saved_stdin, saved_stdout);
        //ft_freetab(split_line); obsolete ?
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static int backup_manager(char **split_line, int *saved_stdin, int *saved_stdout, t_cmd *cmd)
{

    (void)split_line;
    if (backup_fd(saved_stdin, saved_stdout) < 0)
        return EXIT_FAILURE;

    if (redirect_manager(split_line, *saved_stdin, *saved_stdout, cmd) == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int ft_builtin(char **line_parsed, t_cmd *cmd)
{
    int saved_in;
    int saved_out;
    int ret;

    if (backup_manager(line_parsed, &saved_in, &saved_out, cmd) == EXIT_SUCCESS)
        ret = builtin_commands(line_parsed, cmd, saved_in, saved_out);
    else
        ret = EXIT_FAILURE;
    return ret;
}

