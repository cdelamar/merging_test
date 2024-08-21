#include "../includes/parsing.h"

#include "executing.h"

volatile int g_var;

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

static int init_shell_exec(t_cmd **cmd, char **envp)
{
    if (malloc_structs(cmd) != 0)
    {
        ft_putendl_fd(MALLOC_FAILURE, 2);
        return 1;
    }
    (*cmd)->env = envp;
    (*cmd)->heredoc_processed = FALSE;
    return 0;
}


static void process_input(char *line, t_cmd *cmd)
{
    if (line == NULL)
    {
        printf("CTRL + D from shell_loop\n");
        free_structs(cmd);
        exit(0); // Handle exit on EOF (CTRL + D)
    }

    if (*line)
        add_history(line);

    if (execute(line, cmd) == EXIT_COMMAND)
    {
        printf("FREE by EXIT COMMAND (shell_loop)\n");
        free_structs(cmd);
        free(line);
        exit(0); // Handle explicit exit command
    }
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

    path_main(*token_list, envp);
    return 1;
}

void shell_exec_loop(char **envp)
{
    char *line;
    char ** final_tab;
    t_cmd *cmd;
    t_token *token_list;

    while (1)
    {
        if (init_shell_exec(&cmd, envp) != 0)
            return;

        line = readline("MINISHELL>");
        if (line == NULL)
        {
            free_structs(cmd);
            break;
        }

        // Parsing
        token_list = NULL;
        if (parse_input(line, envp, &token_list))
        {
            final_tab = main_cat(&token_list);
            print_free_tab(final_tab); // Exemple a ajuster
            // print_node(token_list);

            // Executing
            process_input(line, cmd);

            token_lstclear(&token_list, free);
        }
        cleanup(line, cmd);
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)argv;

    if (argc != 1)
    {
        printf("MiniSnail is supposed to work by typing './minishell'\n");
        return (0);
    }
    print_snail();
    // Setup signal handlers
    signals();

    // Start shell loop
    shell_exec_loop(envp);

    return 0;
}
