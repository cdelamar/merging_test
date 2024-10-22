/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:23:45 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/18 12:58:45 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

extern sig_atomic_t g_signal;

static char *ft_strjoin_path(const char *path, const char *cmd) // rejouter les fonctions de la lib apres
{
	char *full_path;
	int path_len = strlen(path);
	int cmd_len = strlen(cmd);

	full_path = malloc(path_len + cmd_len + 2); // +2 for '/' and '\0'
	if (!full_path)
		return (NULL);
	strcpy(full_path, path);
	full_path[path_len] = '/';    // ft_strjoin mais avec un slash
	strcpy(full_path + path_len + 1, cmd); // Add the command
	return (full_path);
}

// Helper function to check if a file is executable
static int is_executable(char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0 && sb.st_mode & S_IXUSR) // Check if file exists and is executable
		return (1);
	return (0);
}

// The get_command_path function
static char *get_command_path(char *cmd_name, char **env)
{
	char	**path_dirs;
	char	*path_env;
	char	*cmd_path;
	int		i;

	i = 0;
	// If the command is an absolute or relative path, return it directly
	if (cmd_name[0] == '/' || cmd_name[0] == '.')
	{
		if (is_executable(cmd_name))
			return (strdup(cmd_name));
		return (NULL); // Command given as a path but isn't executable
	}

	// Get the PATH variable from the environment
	path_env = NULL;
	for (i = 0; env[i]; i++)
	{
		if (strncmp(env[i], "PATH=", 5) == 0)
		{
			path_env = env[i] + 5; // Skip "PATH="
			break;
		}
	}
	if (!path_env)
		return (NULL); // PATH variable not found in the environment

	// Split the PATH variable into directories
	path_dirs = ft_split(path_env, ':'); // Assuming ft_split splits a string by a delimiter (':')
	if (!path_dirs)
		return (NULL);

	// boucle for a virer plus tard mais la ca marche
	for (i = 0; path_dirs[i]; i++)
	{
		cmd_path = ft_strjoin_path(path_dirs[i], cmd_name);
		if (cmd_path && is_executable(cmd_path))
		{
			ft_freetab(path_dirs);  // Free the PATH directories array
			return (cmd_path);      // Return the full path of the command
		}
		free(cmd_path); // Free the generated path if it's not executable
	}

	// Clean up and return NULL if the command wasn't found
	ft_freetab(path_dirs);
	return (NULL);
}


int	pipe_execute(t_cmd *cmd)
{
	int		fd[2];     // fd de base
	int		fd_in;     // input trackeur pour bondir dune commande a une autre
	pid_t	pid;
	int		i = 0;
	char	**commands; // To hold all commands split by '|'

	// Split cmd->final_line into separate commands (by '|')
	commands = ft_split(cmd->final_line, '|');
	if (!commands)
		return (0);  // Handle memory error case

	fd_in = 0;  //  STDIN

	signal(SIGPIPE, SIG_IGN); // ignore sigpipe pour parent (IGN parent / DFL child)

	while (commands[i] != NULL)
	{
		// Create a pipe
		if (pipe(fd) == -1)
		{
			perror("pipe");
			return (0);  // Handle pipe error
		}

		// Fork a child process for the current command
		if ((pid = fork()) == -1)
		{
			perror("fork");
			return (0);  // Handle fork error
		}

		if (pid == 0)  // Child process
		{

			signal(SIGPIPE, SIG_DFL); // ignore SIGPIPE pour le child
			if (fd_in != 0)
			{
				dup2(fd_in, STDIN_FILENO);  // Replace stdin with fd_in
				close(fd_in);               // Close original fd_in
			}

			// Redirect output if it's not the last command
			if (commands[i + 1] != NULL)
			{
				dup2(fd[1], STDOUT_FILENO);
			}

			// Close the pipe fds in the child process
			close(fd[0]);  // Close pipe read end
			close(fd[1]);  // Close pipe write end

			// Execute the command
			cmd->path_command = ft_split(commands[i], ' '); // Split command into args
			char *full_path = get_command_path(cmd->path_command[0], cmd->env);

			if (full_path == NULL) // sans cette partie, une commande comme "ls | dqwqwdqdw | wc" fonctionne comme si de rien etait
			{
				fprintf(stderr, "%s: command not found\n", cmd->path_command[0]);
				ft_freetab(cmd->path_command);
				//ft_freetab(cmd->path_split);
				ft_freetab(cmd->final_tab);
				ft_freetab(cmd->env);
				ft_freetab(commands);
				free(cmd->final_line);
				free(full_path);
				free(cmd);
				exit(127);  // ou g_signal
			}


			if (execve(full_path, cmd->path_command, cmd->env) == -1)
			{
				printf("%s : command introuvable\n", cmd->path_command[i]);
				ft_freetab(cmd->path_command);
				exit(EXIT_FAILURE);  // Handle execve error
			}


/*
			execve(full_path, cmd->path_command, cmd->env);

			printf("marche pas\n");
			//printf("%s : command introuvable\n", cmd->path_command[i]);
			ft_freetab(cmd->path_command);
			exit(EXIT_FAILURE);  // Handle execve return*/
		}

		else  // Parent process
		{
			close(fd[1]); // close le write
			if(fd_in != 0)
				close(fd_in); // close de fd precedent
			fd_in = fd[0]; // pour la prochaine
		}

		i++;  // Move to the next command in the pipeline
	}

	// Wait childs
	if(fd_in != 0)
		close(fd_in);

	while (waitpid(-1, NULL, 0) > 0);
	ft_freetab(commands);

	return (0);
}




