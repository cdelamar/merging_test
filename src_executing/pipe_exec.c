/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:23:45 by cdelamar          #+#    #+#             */
/*   Updated: 2024/11/16 01:21:58 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_strjoin_path(char *path, char *cmd)
{
	char	*full_path;
	int		path_len;
	int		cmd_len;

	path_len = ft_strlen(path);
	cmd_len = ft_strlen(cmd);
	full_path = malloc(path_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, path);
	full_path[path_len] = '/';
	ft_strcpy(full_path + path_len + 1, cmd);
	return (full_path);
}

static char	*get_cmd_path(char *cmd_name, char **env)
{
	char	**path_dirs;
	char	*path_env;
	char	*cmd_path;
	int		i;

	i = 0;
	if (cmd_name[0] == '/' || cmd_name[0] == '.')
	{
		if (is_executable(cmd_name))
			return (strdup(cmd_name));
		return (NULL);
	}
	path_env = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path_env = env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
		return (NULL);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (NULL);
	i = 0;
	while (path_dirs[i])
	{
		cmd_path = ft_strjoin_path(path_dirs[i], cmd_name);
		if (cmd_path && is_executable(cmd_path))
		{
			ft_freetab(path_dirs);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	ft_freetab(path_dirs);
	return (NULL);
}

static int	check_all_commands_executable(char ***commands, char **env)
{
	int		i;
	char	*path;

	i = 0;
	while (commands[i] != NULL)
	{
		path = get_cmd_path(commands[i][0], env);
		if (path == NULL)
		{
			if (ft_strcmp(commands[i][0], "<") == 0 ||
				ft_strcmp(commands[i][0], ">") == 0 ||
				ft_strcmp(commands[i][0], "<<") == 0 ||
				ft_strcmp(commands[i][0], ">>") == 0)
				printf("redirections : syntax error\n");
			else
				fprintf(stderr, "%s: command not found\n", commands[i][0]);
			return (0);
		}
		free(path);
		i++;
	}
	return (1);
}

int	pipe_execute(t_cmd *cmd)
{
	int		fd[2];
	int		fd_in;
	pid_t	pid;
	int		i;
	char	*full_path;
	char	***commands;

	fd_in = 0;
	i = 0;
	commands = split_commands(cmd->final_tab);
	if (!commands)
		return (EXIT_FAILURE);
	if (!check_all_commands_executable(commands, cmd->env))
	{
		free_commands(commands);
		return (EXIT_FAILURE);
	}
	while (commands[i] != NULL)
	{
		if (strcmp(commands[i][0], ".") == 0 ||
			strcmp(commands[i][0], "..") == 0)
		{
			fprintf(stderr, "%s: file argument required\n", commands[i][0]);
			g_signal = 127;
			i++;
			continue ;
		}
		if (pipe(fd) == -1)
		{
			free_commands(commands);
			return (EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			free_commands(commands);
			return (EXIT_FAILURE);
		}
		if (pid == 0)
		{
			signal(SIGPIPE, SIG_DFL);
			if (fd_in != 0)
			{
				dup2(fd_in, STDIN_FILENO);
				close(fd_in);
			}
			if (commands[i + 1] != NULL)
				dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			cmd->path_command = commands[i];
			if (handle_redirections(cmd->path_command, 0, cmd) == EXIT_FAILURE)
			{
				fprintf(stderr, "Error handling redirections\n");
				free_cmd_resources(cmd);
				free_commands(commands);
				exit(EXIT_FAILURE);
			}
			if (is_builtin(cmd->path_command[0]))
			{
				if (pipe_builtin(cmd, cmd->path_command) == EXIT_SUCCESS)
				{
					free_cmd_resources(cmd);
					free_commands(commands);
					exit(EXIT_SUCCESS);
				}
				else
				{
					free_cmd_resources(cmd);
					free_commands(commands);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				full_path = get_cmd_path(cmd->path_command[0], cmd->env);
				if (full_path == NULL)
				{
					fprintf(stderr, "%s: command not found\n",
						cmd->path_command[0]);
					free_cmd_resources(cmd);
					free_commands(commands);
					exit(127);
				}
				if (execve(full_path, cmd->path_command, cmd->env) == -1)
				{
					perror("minishell");
					free_cmd_resources(cmd);
					free(full_path);
					free_commands(commands);
					exit(127);
				}
				free(full_path);
			}
		}
		else
		{
			close(fd[1]);
			if (fd_in != 0)
				close(fd_in);
			fd_in = fd[0];
			waitpid(pid, &cmd->status, 0);
		}
		i++;
	}
	if (fd_in != 0)
		close(fd_in);
	while (waitpid(-1, NULL, 1) > 0)
		;
	free_commands(commands);
	return (EXIT_SUCCESS);
}
