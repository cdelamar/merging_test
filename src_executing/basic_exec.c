/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:24:07 by cdelamar          #+#    #+#             */
/*   Updated: 2024/11/26 21:45:36 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*bool is_redirection (char *line)
{
	return (ft_strcmp(line, ">") == 0 || ft_strcmp(line, ">>") == 0 
		|| ft_strcmp(line, "<") == 0 || ft_strcmp(line, "<<") == 0);
}*/

bool	syntax_redirect(char *line)
{
	int		i;
	char	**split_line;

	i = 0;
	split_line = ft_split(line, ' ');

	// printf ("dans syntax redirect:\n");
	// print_tab(split_line);
	while (split_line[i] != NULL)
	{
		if (is_redirection(split_line[i]) && is_redirection(split_line[i + 1]))
		{
			printf("bash: syntax error near unexpected token '%s'\n", split_line[i + 1]);
			ft_freetab(split_line);
			g_signal = 2;
			return (false);
		}
		i++;
	}
	ft_freetab(split_line);
	return (true);
}

int	ft_path_split(t_cmd *cmd)
{
	ft_path(cmd);
	if (!cmd->path)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	basic_parent_process(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		printf("waitpid -1\n");
		g_signal = 1;
		return (EXIT_FAILURE);
	}
	if (WIFEXITED(status))
		g_signal = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_signal = 128 + WTERMSIG(status);
	return (EXIT_SUCCESS);
}

int	basic_child_process(t_cmd *cmd)
{
	char	**split_line;
	char	*command;

	split_line = cmd->final_tab;
	if (validate_redirections(cmd->final_tab) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (preprocess_redirections(cmd->final_tab, cmd) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (!split_line)
		return (EXIT_FAILURE);
	if (handle_redirections(split_line, HEREDOC_ON, cmd) != 0)
	{
		ft_freetab(split_line);
		return (EXIT_FAILURE);
	}
	command = cmd_finder(split_line, cmd);
	if (command)
	{
		close(cmd->fd[1]);
		execve(command, split_line, cmd->env);
	}
	else
	{
		printf("%s: command not found\n", split_line[0]);
		token_lstclear(&cmd->tokens, free);
		g_signal = 127;
	}
	return (child_failure_signal(cmd, command, split_line));
}

int	basic_execute(t_cmd *cmd, t_token **token_list)
{
	int	exit_code;
	if (basic_setup(cmd) != EXIT_SUCCESS)
		return (g_signal);
	exit_code = ft_path_split(cmd);
	if (is_builtin(cmd->final_tab[0]) == true)
	{
		close(cmd->fd[0]);
		close(cmd->fd[1]);
		return (ft_builtin(cmd, token_list));
	}
	if (exit_code != EXIT_SUCCESS)
		return (path_split_return(cmd));
	cmd->pid1 = fork();
	if (cmd->pid1 < 0)
		return (fork_error(cmd));
	else if (cmd->pid1 == 0)
		exit (ft_child(cmd, exit_code, token_list));
	else
		return (ft_parent(cmd, exit_code));
}
