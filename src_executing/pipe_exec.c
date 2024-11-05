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

void free_commands(char ***commands)
{
    int i = 0;
    while (commands[i])
    {
        int j = 0;
        while (commands[i][j])
        {
            free(commands[i][j]);
            j++;
        }
        free(commands[i]);
        i++;
    }
    free(commands);
}

static void free_cmd_resources (t_cmd *cmd)
{
	//ft_freetab(cmd->path_command);
	ft_freetab(cmd->final_tab);
	ft_freetab(cmd->env);
	free(cmd->final_line);
	//free(full_path);
	token_lstclear(&cmd->tokens, free);
	free(cmd);
}

//--------------------------

static int count_commands(char **final_tab)
{
    int count = 1;
    int i = 0;

    while (final_tab[i])
    {
        if (strcmp(final_tab[i], "|") == 0)
            count++;
        i++;
    }
    return count;
}


static int count_args(char **final_tab, int start)
{
    int count = 0;
    int i = start;

    while (final_tab[i] && strcmp(final_tab[i], "|") != 0)
    {
        count++;
        i++;
    }
    return count;
}

static char	***split_commands(char **final_tab)
{
	int		num_commands = count_commands(final_tab);
	char	***commands = malloc((num_commands + 1) * sizeof(char **));
	int		cmd_index = 0;
	int		arg_index = 0;

	if (!commands)
		return (NULL);

	for (int i = 0; final_tab[i]; i++)
	{
		if (strcmp(final_tab[i], "|") == 0)
		{
			// Null-terminate the current command and move to the next
			commands[cmd_index][arg_index] = NULL;
			cmd_index++;
			arg_index = 0;
			continue;
		}

		//technique : alloue pour l'argument actuel
		if (arg_index == 0)
			commands[cmd_index] = malloc((count_args(final_tab, i) + 1) * sizeof(char *));

		// Copier argument to the current command
		commands[cmd_index][arg_index] = strdup(final_tab[i]);
		arg_index++;
	}

	// "securite"
	commands[cmd_index][arg_index] = NULL;
	commands[cmd_index + 1] = NULL;

	return (commands);
}

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

int pipe_execute(t_cmd *cmd)
{
    int fd[2];
    int fd_in = 0;
    pid_t pid;
    int i = 0;

    char ***commands = split_commands(cmd->final_tab);
    if (!commands)
        return (EXIT_FAILURE);

    while (commands[i] != NULL)
	{
        if (pipe(fd) == -1)
		{
            free_commands(commands);
            return (EXIT_FAILURE);
        }

        if ((pid = fork()) == -1)
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
				// waitpid(pid, &cmd->status, 0);
				// if (WIFSIGNALED(cmd->status) && WTERMSIG(cmd->status) == SIGPIPE)
				// {
					// fprintf(stderr, "wrong input\n");
					//break; // ?
				// }
				// i++;
                char *full_path = get_command_path(cmd->path_command[0], cmd->env);
                if (full_path == NULL)
				{
                    fprintf(stderr, "%s: command not found\n", cmd->path_command[0]);
                    free_cmd_resources(cmd);
                    free_commands(commands);
                    exit(127);
                }
                if (execve(full_path, cmd->path_command, cmd->env) == -1)
				{
                    perror("execve");
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
            close(fd[1]);  // Close la partie write
            if (fd_in != 0)
                close(fd_in);  // Close le read d'avant
            fd_in = fd[0];  // sauvegarde pour la suite des comds
            i++;
        }
    }

    while (waitpid(-1, NULL, 0) > 0);
    free_commands(commands);
    return (EXIT_SUCCESS);
}
