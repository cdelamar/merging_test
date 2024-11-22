/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:43:11 by laubry            #+#    #+#             */
/*   Updated: 2024/11/22 18:19:35 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_exit_cleanup(char **split_line, t_cmd *cmd,
		int saved_in, int saved_out)
{
	ft_freetab(split_line);
	token_lstclear(&cmd->tokens, free);
	ft_freetab(cmd->env);
	if(cmd->path_split)
		ft_freetab(cmd->path_split);
	free(cmd->final_line);
	free(cmd);
	restore_fd(saved_in, saved_out);
	exit(g_signal);
}

static int	handle_exit_builtin(char **split_line, t_cmd *cmd,
		int saved_in, int saved_out)
{
	int	exit_code;
	exit_code = ft_exit(split_line, cmd);
	if (exit_code == -1)
	{
		return (0);
	}
	if (cmd->pid1 == 0)
	{
		handle_exit_cleanup(split_line, cmd, saved_in, saved_out);
	}
	return (exit_code);
}

static int	handle_builtins(char **split_line, t_cmd *cmd)
{
	if (ft_strcmp(split_line[0], "unset") == 0)
		return (ft_unset(split_line, cmd));
	else if (ft_strcmp(split_line[0], "echo") == 0)
		return (ft_echo(split_line));
	else if (ft_strcmp(split_line[0], "cd") == 0)
		return (ft_cd(split_line[1]));
	else if (ft_strcmp(split_line[0], "export") == 0)
		return (ft_export(split_line, cmd));
	else if (ft_strcmp(split_line[0], "env") == 0)
		return (ft_env(cmd));
	else if (ft_strcmp(split_line[0], "pwd") == 0)
		return (ft_pwd());
	return (EXIT_FAILURE);
}

int	builtin_commands(char **split_line, t_cmd *cmd,
		int saved_in, int saved_out)
{
	int	ret;

	ret = handle_builtins(split_line, cmd);
	if (ft_strcmp(split_line[0], "exit") == 0)
		ret = handle_exit_builtin(split_line, cmd, saved_in, saved_out);
	restore_fd(saved_in, saved_out);
	return (ret);
}
