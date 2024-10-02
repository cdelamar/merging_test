#include "../includes/parsing.h"
#include "executing.h"

volatile sig_atomic_t  g_signal;

// __________________________________
// _________________ EXECUTING
// __________________________________

void print_tab(char **tab)
{
	int i;

	i = 0;
	while(tab[i])
	{
		printf("tab %d : %s\n", i, tab[i]);
		i++;
	}
	return ;
}

static void process_input(char *line, t_cmd *cmd)
{
	if (line == NULL)
	{
		printf("CTRL + D from shell_loop\n");
		ft_freetab(cmd->env);
		free_structs(cmd);
		exit(0); // Handle exit on EOF (CTRL + D)
	}

	if (space_only(line) == true)
	{
		//free_structs(cmd);
		return;
	}

	if (*line)
		add_history(line);

	execute(line,cmd); // execute retourne un int mais je men sert pas
}

char **cpy_tab(char **dest, char **src)
{
	int len = -1;
	while (src[++len])
		dest[len] = ft_strdup(src[len]);
	dest[len] = NULL;
	return dest;
}

// __________________________________
// _________________ PARSING
// __________________________________

int	add_node(t_token **token_list, char **strs, int i)
{
	t_token	*new_node;

	new_node = token_lstnew(strs[i]);
	if (new_node == NULL)
		return (check_error(ERROR_NODE));
	new_node->index = i;
	token_lstadd_back(token_list, new_node);
	return (1);
}

int	make_token(char **split_line, t_token **token_list)
{
	int	i;
	int	argc;

	argc = nbr_of_strs(split_line);
	if (argc == 0)
		return (0);
	i = 0;

	while (i < argc)
	{
		if (!add_node(token_list, split_line, i))
			return (0);
		i++;
	}
	lexer(*token_list);
	return (1);
}

void free_split_line(char **split_line)
{
	int	i;

	i = 0;
	while (split_line[i])
	{
		free(split_line[i]);
		i++;
	}
	free(split_line);
}

int parse_input(char *line, char **envp, t_token **token_list)
{
	char **split_line;

	split_line = ft_split_boosted(line);
	if (split_line == NULL)
		return 0;

	if (solo_quote(split_line) || badchar(split_line))
	{
		free_split_line(split_line);
		return 0;
	}

	if (!make_token(split_line, token_list))
	{
		free_split_line(split_line);
		return 0;
	}

	free(split_line);
	path_main(*token_list, envp);
	return 1;
}
// __________________________________
// _________________ MERGE
// __________________________________

void shell_loop(char **envp)
{
	char *line;
	t_cmd *cmd;
	t_token *token_list;
	int	len = -1;

	while (envp[++len]) {}
	char **tab = malloc(sizeof(char *) * (len + 1)); // allocate memory to copy envp
	cpy_tab(tab, envp); // copy envp to cmd->env

	cmd = NULL;
	while (1)
	{
		printf("whille loop here\n");
		if (malloc_structs(&cmd) != 0)
		{
			ft_putendl_fd(MALLOC_FAILURE, 2);
			return ;
		}
		cmd->heredoc_processed = FALSE;
		cmd->env = tab;
		line = readline("$ ");

		if (line == NULL) // ctrl + D
		{
			printf("CTRL + D from shell_loop\n");
			ft_freetab(cmd->env);
			free_structs(cmd);
			exit(0); // Handle exit on EOF (CTRL + D)
		}


	token_list = NULL;
		if (parse_input(line, envp, &token_list))
		{
			cmd->line_parsed = main_cat(&token_list);
		// Executing

			printf("process input\n");
			process_input(line, cmd);
			token_lstclear(&token_list, free);
			//ft_freetab(line_parsed);
		}
		//cleanup(line, cmd);
		free(line);
		tab = cmd->env;
	}
}

int main(int argc, char **argv, char **envp)
{
	g_signal = 0;
	(void)argv;

	if (argc != 1)
	{
		printf("incorrect arguments\n");
		return (0);
	}

	rl_outstream = stderr; // implemented due to mstest requirements
	signals();
	// Start shell loop
	printf("shell loop\n");
	shell_loop(envp); // function where parsing and executing are merged

	return 0;
}
