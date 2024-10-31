/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:23:45 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/22 16:44:09 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern sig_atomic_t	g_signal;

static char	*ft_strjoin_path(const char *path, const char *cmd)
{
	char	*full_path;
	int		path_len;
	int		cmd_len;

	path_len = strlen(path);
	cmd_len = strlen(cmd);
	full_path = malloc(path_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	strcpy(full_path, path);
	full_path[path_len] = '/';
	strcpy(full_path + path_len + 1, cmd);
	return (full_path);
}

static int	is_executable(char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0 && sb.st_mode & S_IXUSR)
		return (1);
	return (0);
}

static char	*get_command_path(char *cmd_name, char **env)
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
		if (strncmp(env[i], "PATH=", 5) == 0)
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

int	pipe_execute(t_cmd *cmd)
{
	int		fd[2];
	int		fd_in;
	pid_t	pid;
	int		i;
	char	**commands;

	i = 0;
	commands = ft_split(cmd->final_line, '|');
	if (!commands)
		return (0);
	fd_in = 0;
	//signal(SIGPIPE, SIG_IGN);
	while (commands[i] != NULL)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe");
			return (0);
		}
		if ((pid = fork()) == -1)
		{
			perror("fork");
			return (0);
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
			{
				dup2(fd[1], STDOUT_FILENO);
			}

			close(fd[0]);
			close(fd[1]);

			cmd->path_command = ft_split(commands[i], ' ');
			if (!cmd->path_command)
				exit(1); // on gerera les free plus tard

			//printf("cmd->path_command\n");
			//print_tab(cmd->path_command);

			if(is_builtin(cmd->path_command[0]))
			{
				//printf("in builtin\n");
				if (pipe_builtin(cmd, cmd->path_command) == EXIT_SUCCESS)
				{
					ft_freetab(cmd->path_command);
					ft_freetab(cmd->final_tab);
					ft_freetab(cmd->env);
					ft_freetab(commands);
					free(cmd->final_line);
					token_lstclear(&cmd->tokens, free);
					free(cmd);
                    exit(EXIT_SUCCESS);
                }

				else // a revoir
				{
				//	printf("echec builtin\n");
					ft_freetab(cmd->path_command);
					ft_freetab(cmd->final_tab);
					ft_freetab(cmd->env);
					ft_freetab(commands);
					free(cmd->final_line);
					token_lstclear(&cmd->tokens, free);
					free(cmd);
                    exit(EXIT_SUCCESS);
				}
			}

			else
			{
				char *full_path = get_command_path(cmd->path_command[0], cmd->env);
				if (full_path == NULL)
				{
					fprintf(stderr, "%s:command not found\n", cmd->path_command[0]);
					ft_freetab(cmd->path_command);
					ft_freetab(cmd->final_tab);
					ft_freetab(cmd->env);
					ft_freetab(commands);
					free(cmd->final_line);
					free(full_path);
					token_lstclear(&cmd->tokens, free);
					free(cmd);
					exit(127);
				}
				if (execve(full_path, cmd->path_command, cmd->env) == -1)
				{
					printf("%s : command not found\n", cmd->path_command[0]); // 0 ou i je pige plus
					ft_freetab(cmd->path_command);
					exit(127);
				}
			}
		}
		else
		{
			close(fd[1]);
			if (fd_in != 0)
				close(fd_in);
			fd_in = fd[0];

			waitpid(pid, &cmd->status, 0);  // Wait for child process

			//_____________

			//_____________

            if (WIFSIGNALED(cmd->status) && WTERMSIG(cmd->status) == SIGPIPE)
            {
                fprintf(stderr, "aie aie aie\n");
            }

			//if(WIFEXITED(cmd->status))
			//	cmd->exit_status = WEXITSTATUS(cmd->status);
		}
		i++;
	}
	if (fd_in != 0)
		close(fd_in);
	//while (waitpid(-1, NULL, 0) > 0);
	ft_freetab(commands);
	return (0);
}
