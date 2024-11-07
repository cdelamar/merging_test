/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laubry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:43:11 by laubry            #+#    #+#             */
/*   Updated: 2024/10/22 18:10:01 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool is_builtin(char *command)
{
    if (!command)
        return false;
    return (ft_strcmp(command, "unset") == 0 ||
            ft_strcmp(command, "echo") == 0 ||
            ft_strcmp(command, "cd") == 0 ||
            ft_strcmp(command, "export") == 0 ||
            ft_strcmp(command, "env") == 0 ||
            ft_strcmp(command, "pwd") == 0 ||
            ft_strcmp(command, "exit") == 0);
}


static int	builtin_commands(char **split_line, t_cmd *cmd,
		int saved_in, int saved_out)
{
	int	ret;
	int	exit_code;

	ret = EXIT_FAILURE;
	if (ft_strcmp(split_line[0], "unset") == 0)
		ret = ft_unset(split_line, cmd);
	else if (ft_strcmp(split_line[0], "echo") == 0)
		ret = ft_echo(split_line);
	else if (ft_strcmp(split_line[0], "cd") == 0)
		ret = ft_cd(split_line[1]);
	else if (ft_strcmp(split_line[0], "export") == 0)
		ret = ft_export(split_line, cmd);
	else if (ft_strcmp(split_line[0], "env") == 0)
		ret = ft_env(cmd);
	else if (ft_strcmp(split_line[0], "pwd") == 0)
		ret = ft_pwd();
	else if (ft_strcmp(split_line[0], "exit") == 0)
	{
		printf ("FT_EXIT CALL\n");
		exit_code = ft_exit(split_line, cmd);
		if(exit_code == -1)
			return(0);
		if (cmd->pid1 == 0)
		{
			ft_freetab(split_line);
			token_lstclear(&cmd->tokens, free);
			//ft_freetab(cmd->final_tab);
			ft_freetab(cmd->env);
			ft_freetab(cmd->path_split);
			free(cmd->final_line);
			free(cmd);
			restore_fd(saved_in, saved_out);
			exit(exit_code);
		}
		ret = exit_code;
	}
	restore_fd(saved_in, saved_out);
	return (ret);
}

static int	redirect_manager(char **split_line, int saved_stdin,
		int saved_stdout, t_cmd *cmd)
{
	if (handle_redirections(split_line, 0, cmd) == EXIT_FAILURE)
	{
		restore_fd(saved_stdin, saved_stdout);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	backup_manager(char **split_line, int *saved_stdin,
		int *saved_stdout, t_cmd *cmd)
{
	(void)split_line;
	if (backup_fd(saved_stdin, saved_stdout) < 0)
		return (EXIT_FAILURE);
	if (redirect_manager(split_line, *saved_stdin, *saved_stdout, cmd)
		== EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_builtin(t_cmd *cmd)
{
	char	**split_line;
	int		saved_in;
	int		saved_out;
	int		ret;

	split_line = cmd->final_tab;
	//printf("split_line\n");
	//print_tab(split_line);
	if (!split_line)
		return (EXIT_FAILURE);
	if (backup_manager(split_line, &saved_in, &saved_out, cmd) == EXIT_SUCCESS)
		ret = builtin_commands(split_line, cmd, saved_in, saved_out);
	else
		ret = (EXIT_FAILURE);
	//ft_freetab(split_line);
	return (ret);
}

int pipe_builtin(t_cmd *cmd, char **command)
{
	int		saved_in;
	int		saved_out;
	int		ret;

	if (backup_manager(command, &saved_in, &saved_out, cmd) == EXIT_SUCCESS)
		ret = builtin_commands(command, cmd, saved_in, saved_out);
	else
		ret = (EXIT_FAILURE);
	return (ret);
}
