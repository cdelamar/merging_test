/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Laubry <aubrylucas.pro@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 13:11:04 by laubry            #+#    #+#             */
/*   Updated: 2024/08/14 19:03:39 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

// TODO : once merge is fully implmented
// adding all new function to lib
// adding all new files to makefile
// adding lucas lib to the original one, or create one especially for it
// -lib
// -parse_lib

volatile int	g_var = 0;

/*
dans split_dop/verif_error la fonction badchar dedans a la fin cest ca qui faut gerer
en gros si il y a "" ou '' tu envoie a clemment () bah rien
et les return dans les main il faut les suprimer et gere les erreur sans stop le mini
shell*/



//faire un check avant tout de verifier si lutilisateur a mis les caractere \x01 ou \x02
//le cas echo "caca"|oui et bien les double quote ne fusione pas avec le pipe pareil pour les > ex..
//le realloc tu doit le recoder dans parsing.c
//enlever les returne 0 en cas derreur pour ne pas quiter les minishell


extern volatile int g_var;

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

static void process_input(char *line, t_cmd *cmd, t_token **token_list, char **envp)
{
    char **split_line;
    char **final_tab;

    if (line == NULL)
    {
        printf("CTRL + D from shell_loop\n");
        free_structs(cmd);
        exit(0); // Handle exit on EOF (CTRL + D)
    }

    if (*line)
        add_history(line);

    // Parsing logic
    split_line = ft_split_boosted(line);
    if (split_line == NULL || solo_quote(split_line) || badchar(split_line))
    {
        free_split_line(split_line);
        return;
    }

    if (!make_token(split_line, token_list))
    {
        free_split_line(split_line);
        return;
    }

    path_main(*token_list, envp);
    final_tab = main_cat(token_list);

    print_free_tab(final_tab);
    print_node(*token_list);

    // Execution logic
    if (execute(line, cmd) == EXIT_COMMAND)
    {
        printf("FREE by EXIT COMMAND (shell_loop)\n");
        free_structs(cmd);
        free_split_line(split_line);
        free(line);
        exit(0); // Handle explicit exit command
    }

    cleanup(line, cmd);
    free_split_line(split_line);
    token_lstclear(token_list, free); // Clear token list to prevent memory leaks
}

void shell_loop(char **envp)
{
    char *line;
    t_cmd *cmd;
    t_token *token_list = NULL;

    while (1)
    {
        if (init_shell_exec(&cmd, envp) != 0)
            return;

        line = readline("MINISHELL>");
        process_input(line, cmd, &token_list, envp);
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)argv;

    if (argc != 1)
    {
        printf("MiniSnail is supposed to work by typing './minishell'\n");
        return 0;
    }

    // Setup signal handlers
    signals();

    // Start shell loop
    shell_loop(envp);

    return 0;
}


/*
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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**split_line;
	char	**final_tab;
	t_token	*token_list;

	token_list = NULL;
	(void)argv;
	signals();
	if (argc > 1)
		return (check_error(ERROR_ARGS));
	print_snail();
	while (1)
	{
		line = readline("minishell>");
		split_line = ft_split_boosted(line);
		if (split_line == NULL || solo_quote(split_line) || badchar(split_line))
			return (0);
		print_split(split_line);
		if (line == NULL)
		{
			free_split_line(split_line);
			return (0);
		}
		if (!make_token(split_line, &token_list))
		{
			free_split_line(split_line);
			return (0);
		}
		path_main(token_list, envp);
		final_tab = main_cat(&token_list);

		print_free_tab(final_tab);
		print_node(token_list); // il leaks mais tkt il est pas dans le code
		//free_split_line(split_line)
		free(split_line);
		free(line);
		token_lstclear(&token_list, free);// pause des leaks
	}
}*/



