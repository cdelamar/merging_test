/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 13:11:04 by laubry            #+#    #+#             */
/*   Updated: 2024/10/03 11:37:06 by cdelamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

volatile int	g_signal = 0;

/*
char *ft_realloc_string(char *str, int size)
{
	char *res = malloc(size);
	int i = -1;
	while(str[++i])
		res[i] = str[i];
	res[i] = 0;
	free(str);
	return res;
}*/

char *ft_realloc_string(char *str, int new_size)
{
    char *res;
    int old_len = strlen(str);

    res = malloc(old_len + new_size);
    if (!res)
        return NULL;

    // Copy the original string to the new buffer
    int i = 0;
    while (str[i])
    {
        res[i] = str[i];
        i++;
    }
    res[i] = '\0'; // Null-terminate the new string

    free(str); // Free the old string
    return res;
}
char *tab_to_str(char **tab)
{
	print_tab(tab);
    int i = -1;
    char *str = malloc(1);
    str[0] = 0;
    while(tab[++i])
    {
        str = ft_realloc_string(str, strlen(tab[i]) + 2);
        ft_strcat(str, tab[i]);
        if(tab[i + 1])			//major fix
			ft_strcat(str, " ");
    }
	printf("str : ..%s..\n", str);
	// peut etre freetab ici aussi
	//ft_freetab(tab)
    return str;
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

int	main(int argc, char **argv, char **envp)
{
///// laubry : init
	char	*line;
	char	**split_line;
	char	**final_tab;
	char	*final_line;
	t_token	*token_list;

	token_list = NULL;
	(void)argv;
	signals();


///// cdelamar : init
	t_cmd	*cmd = NULL;
	int		len = -1;

	while (envp[++len]) {}
	char **tab = malloc(sizeof(char *) * (len + 1));
	cpy_tab(tab, envp);

	if (argc > 1)
		return (check_error(ERROR_ARGS));


	while (1)
	{

		///// cdelamar : init loop
		if (malloc_structs(&cmd) != 0)
		{
			ft_putendl_fd(MALLOC_FAILURE, 2);
			return (0);
		}
		cmd->heredoc_processed = FALSE;
		cmd->env = tab;
		line = readline("$ ");
		split_line = ft_split_boosted(line);
		if (split_line == NULL)
		{
			printf("je sors ici\n");
			ft_freetab(cmd->env);
			return (0);
		}
		if (solo_quote(split_line) || badchar(split_line))
		{
			free_split_line(split_line);
			return (0);
		}
		//print_split(split_line);
		if (line == NULL)
		{
			printf("je sors plutot la\n");
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
		final_line = tab_to_str(final_tab);
		//printf ("the line = %s \n", final_line);
		token_lstclear(&token_list, free);

		free(line);
		free(split_line);
		process_input(final_line, cmd);

		//printf("before free : %s\n", final_line);
		free(final_line);
		//printf("after free : %s\n", final_line);
		tab = cmd->env;
	}
	return (0);
}
