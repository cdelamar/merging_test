/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:43:11 by laubry            #+#    #+#             */
/*   Updated: 2024/11/29 20:23:32 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_exit_cleanup(char **split_line, t_cmd *cmd, int saved[],
	t_token **token_list, char ***to_free)
{
	if (to_free)
		free_commands(to_free);
	(void)split_line;
	token_lstclear(&cmd->tokens, free);
	ft_freetab(cmd->env);
	if (cmd->path_split)
		ft_freetab(cmd->path_split);
	free(cmd->final_line);
	if (cmd->final_tab)
		ft_freetab(cmd->final_tab);
	free(cmd);
	restore_fd(saved[0], saved[1]);
	token_lstclear(token_list, free);
	exit(g_signal);
}

static int	handle_exit_builtin(char **split_line, t_cmd *cmd,
	int saved[], t_token **token_list, char ***to_free)
{
	int	exit_code;

	exit_code = ft_exit(split_line, cmd);
	if (exit_code == -1)
		return (0);
	if (cmd->pid1 == 0)
		handle_exit_cleanup(split_line, cmd, saved, token_list, to_free);
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

int	builtin_commands(char **split_line, t_cmd *cmd, int saved[],
	t_token **token_list, char ***to_free)
{
	int	ret;

	ret = handle_builtins(split_line, cmd);
	if (ft_strcmp(split_line[0], "exit") == 0)
		ret = handle_exit_builtin(split_line, cmd, saved, token_list, to_free);
	restore_fd(saved[0], saved[1]);
	return (ret);
}
