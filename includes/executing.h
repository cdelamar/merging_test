/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 15:05:45 by cdelamar          #+#    #+#             */
/*   Updated: 2024/10/02 20:22:31 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTING_H
# define EXECUTING_H

# include "parsing.h"
# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>
# include <limits.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/uio.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>

# define INIT_SUCCESS	"init succeed\n"
# define FORK_FAILURE	"fork failure\n"
# define PIPE_FAILURE	"pipe failure\n"
# define ARGC_FAILURE	"argc failure\n"
# define CHILD_FAILURE	"child failure\n"
# define IN_FAILURE		"infile failure\n"
# define PARENT_FAILURE	"parent failure\n"
# define MALLOC_FAILURE	"malloc failure\n"
# define OUT_FAILURE	"outfile failure\n"
# define ENV_FAILURE	"environnement failure\n"
# define BUFFER_SIZE	5000

# define HEREDOC_ON		1
# define HEREDOC_OFF	0
# define HEREDOC_TMP_FILE "/tmp/heredoc_tmp"

# define FALSE			0
# define TRUE			1

# define STDIN 0
# define STDOUT 1
# define STDERR 2
# define EXIT_COMMAND	3

typedef struct s_cmd
{
	int		fd[2];
	int		heredoc_processed;
	pid_t	pid1;
	pid_t	pid2;
	int		fd_in;
	int		status;
	int		infile;
	int		outfile;
	char	*path;
	char	**path_split;
	char	**path_command;
	char	**env;
	int		export_added;
	char	**line_parsed;
}	t_cmd;

void				ft_path_command(t_cmd *cmd, char *line); 	// PATH_COMMAND ('ls -lathr' 'wc' 'cat -e'....)
int					ft_path_split(t_cmd *cmd);					// PATH_SPLIT ('usr/local/sbin' '/sbin' 'snap/bin'....)

// excecuting
void 				shell_exec_loop(char **envp); // WIP
void				handle_error(char *msg, t_cmd *cmd, int *fd);
int					execute(char *line, t_cmd *cmd);

// memory
int					malloc_structs(t_cmd **cmd);
void				free_structs(t_cmd *cmd);
void				ft_freetab(char **tab);

// command
char				*cmd_cat(const char *path_split, char *slash,
						char *command);
char				*cmd_finder(char **split_line, t_cmd *cmd);

// envp
int					ft_copy_envp(char **envp, t_cmd *cmd);
char				*path_finder(t_cmd *cmd, char *path, int size);
void				ft_path(t_cmd *cmd);

// builtins
int					ft_unset(t_cmd *cmd);
int					ft_exit(t_cmd *cmd);
int					ft_builtin(char *line, t_cmd *cmd);
int					ft_export(t_cmd *cmd);
int					ft_echo(t_cmd *cmd);
int					ft_env(t_cmd *cmd);
int					ft_cd(char *path);
int					ft_pwd(void);

//signals
void				sigint_handler(int sig);
void				signals(void);

//redirections
int					handle_redirections(char **split_line, int status, t_cmd *cmd);
int					ft_heredoc_redirect(char *delim);
int					ft_input_redirect(char **split_line, int i);
int					ft_output_redirect(char **split_line, int i, int append);
int					ft_heredoc(char *limit);


//fd
void				close_fds(int *fd);
int					backup_fd(int *saved_stdin, int *saved_stdout);
void				restore_fd(int saved_stdin, int saved_stdout);
int					open_file(char *filename, int flags, int mode);
int					open_heredoc_file(void);


//basic executing
int					handle_exit_command(char *line);
int					basic_child_process(char *line, t_cmd *cmd);
int					basic_parent_process(pid_t pid);
int					basic_execute(char *line, t_cmd *cmd);

//pipe executing
void	handle_pipe_error(t_cmd *cmd, int *fd);
void	handle_fork_error(t_cmd *cmd, int *fd);
void	execute_child_process(t_cmd *cmd, int *fd, int i);
void	init_cmd(t_cmd *cmd, char *line);
int		pipe_execute(char *line, t_cmd *cmd);

//safety

bool space_only (char *line);
void print_tab(char **tab);

//WIP
int		handle_path(t_cmd *cmd);
void	cleanup(char *line, t_cmd *cmd);
void    reset_signals(void);
void    heredoc_signals(void);
void sigint_heredoc(int sig);

int ft_isnumber(char *str);
bool syntax_redirect(char *line);

void setup_signal_handler(int signum, void (*handler)(int));
bool freeable_tab (char **tab);
void free_cmd(t_cmd *cmd);

char **cpy_tab(char **dest, char **src);

#endif
