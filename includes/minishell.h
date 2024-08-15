/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 15:05:45 by cdelamar          #+#    #+#             */
/*   Updated: 2024/08/08 13:22:12 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

# define ERROR_NODE 1
# define ERROR_ARGS 2
# define ERROR_QUOTE 3

extern volatile int	g_var;
/* ========== ENUM ================= */

typedef enum token_num
{
	WORD, //commande
	SPACES,
	PIPE, // |
	REDIRECTION, // <<
	VAR, // $
	DOUBLE_QUOTE, //
	SIMPLE_QUOTE,
	LAST_VERIF,
	GLOBAL, // $?
	ERROR, // erreur
	END, // \0
}	t_token_enum;

/* ========== STRUCTURE ============= */

typedef struct s_token
{
	t_token_enum	type; // COMMANDE
	char			*content; // LS..cd ex...
	int				index; // 1 2 3
	struct s_token	*next; // next
}	t_token;

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
	char	*line; //WIP
}	t_cmd;

// excecuting
void 				shell_exec_loop(char **envp); // WIP
void				initialize_cmd(t_cmd *cmd, char *line);
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
int					ft_unset(char **split_line, t_cmd *cmd);
int					ft_exit(char **split_line, t_cmd *cmd);
int					ft_builtin(char *line, t_cmd *cmd);
int					ft_export(char **args, t_cmd *cmd);
int					ft_echo(char **split_line);
int					ft_env(t_cmd *cmd);
int					ft_cd(char *path);
int					ft_pwd(void);

//signals
void				sigint_handler(int sig);
void				signals(void);

//redirections
int					handle_redirections(char **args, int status, t_cmd *cmd);
int					ft_heredoc_redirect(char *delim);
int					ft_input_redirect(char **args, int i);
int					ft_output_redirect(char **args, int i, int append);
int					ft_heredoc(char *limit);

//fd
void				close_fds(int *fd);
int					backup_fd(int *saved_stdin, int *saved_stdout);
void				restore_fd(int saved_stdin, int saved_stdout);
int					open_file(char *filename, int flags, int mode);
int					open_heredoc_file(void);

//basic executing
int					handle_exit_command(char *line);
int					set_command_path(t_cmd *cmd);
int					basic_child_process(char *line, t_cmd *cmd);
int					basic_parent_process(pid_t pid, char **split_line);
int					basic_execute(char *line, t_cmd *cmd);

//pipe executing
void	handle_pipe_error(t_cmd *cmd, int *fd);
void	handle_fork_error(t_cmd *cmd, int *fd);
void	execute_child_process(t_cmd *cmd, int *fd, int i);
void	init_cmd(t_cmd *cmd, char *line);
int		pipe_execute(char *line, t_cmd *cmd);

//WIP
int		handle_path(t_cmd *cmd);
void	cleanup(char *line, t_cmd *cmd);
void    reset_signals(void);
void    heredoc_signals(void);
void sigint_heredoc(int sig);

int ft_isnumber(char *str);


// _____________ PARSING _____________

/* ========== FONCTION ============= */

/*-------------error-----------------*/
int			check_error(int code_error);
int			check_error_before_split(char *line);
int			badchar(char **split_line);

/*-------------libft-----------------*/
t_token		*token_lstnew(void *content);
// void		token_lstadd_back(t_token **lst, t_token *new);
// void		token_lstclear(t_token **lst, void (*del)(void*));
// int			ft_strcmp(char *s1, char *s2);
// int			ft_strncmp(char *s1, char *s2, size_t n);
// size_t		ft_strlen(const char *str);
int			ft_strlen2(char	*word);
// char		*ft_substr(const char *s, unsigned int start, size_t len);
// char		*ft_strchr(char *s, int c);
int			ft_strchr2(char *s, char c);
char		*ft_strncpy(char *dst, char *src, size_t n);
void		ft_bzero(void *s, size_t n);
char		**ft_split_boosted(char *s);
int			check_char(char *s, int i, char **lst, int j);
// int			ft_isalpha(int c);
// char		*ft_strjoin(char *s1, char *s);
// char		*ft_strdup(const char *src);
// char		*ft_strtrim(char const *s1, char const *set);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
char		*ft_strcpy(char *dest, char *src);
// void		ft_lstdelone(t_token *lst, void (*del)(void*));
char		*ft_strdup_for_quote(const char *src);

/*------------tools------------------*/
void		print_node(t_token *token);
int			nbr_of_strs(char **strs);
void		print_path(char **evnp);
void		print_snail(void);
int			is_qte(char c);

/*------tools of environement--------*/
int			find_the_dollar(t_token *token);
int			skip_prefix(char *word);
char		*skip_sufix(char *word);
void		is_dollar_interogation(t_token *token_list);
void		path_other(char **envp, t_token *token_list, int place_of_dollar);
char		**minisplit_dollar(char *word);
int			nbr_of_dollar(t_token *token_list, int place_of_dollar);
int			which_node(int nbr_dollar, t_token *token_list);
char		**delet_space_to_tab(char **tab_token);

/*new*/
void		path_main(t_token *token_list, char **envp);

/*------------parsing----------------*/
void		lexer(t_token *token);
void		path_main(t_token *token_list, char **envp);

/*------------cat_quote--------------*/
void		before_node_cat(t_token **token, int i, int verif);
void		after_node_cat(t_token **token, int i, int verif);
int			copy_node(t_token *head, t_token *new_node);
int			copy_node_after(t_token *head, t_token *new_node, int i_content);
int			copy_mid_node(t_token *head, t_token *new_node, int i_content);
int			copy_mid_after_node(t_token *head, t_token *new_node);
void		advance_to_node(t_token **head, t_token **before, int *i);
char		**delet_quote_inword(t_token **token, t_token *place);
/*----------lstnew_with_cat---------*/
char		**mouve_str(char **str, int i);
int			check_is_space_node(char **str, int i);
int			lstnew_with_cat(char **str, int i);
char		**main_cat(t_token **token);
/*----------tools_for_cat----------*/
char		**malloc_tab(t_token **token);
void		cat_quote(char **tab_token, t_token **token);


/*--------------free-----------------*/
void		free_split(char **strs);
void		free_head(t_token *head);
void		print_free_tab(char **final_token);


size_t		count_word(char *s);
int			is_char(char s);
int			skip_space(char *s, int i);
void		print_split(char **split_line);
int			solo_quote(char **split_line);

void		signals(void);
void		sigint_handler(int sig);

#endif
