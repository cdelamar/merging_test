/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 13:11:04 by laubry            #+#    #+#             */
/*   Updated: 2024/11/06 16:31:45 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void free_token_list(t_token *token_list)
{
    t_token *temp;
    while (token_list)
    {
        temp = token_list;
        token_list = token_list->next;
        free(temp->content);
        free(temp);
    }
}

t_token *copy_token_list(t_token *laubry_list)
{
    t_token *copy_head = NULL;
    t_token *copy_current = NULL;
    t_token *current = laubry_list;

    while (current != NULL)
    {
        t_token *new_node = malloc(sizeof(t_token));
        if (!new_node)
        {
            free_token_list(copy_head);
            return NULL;
        }

        new_node->type = current->type;
        new_node->content = ft_strdup(current->content);
        if (!new_node->content)
        {
            free(new_node);
            free_token_list(copy_head);
            return NULL;
        }

        new_node->index = current->index;
        new_node->next = NULL;

        if (copy_head == NULL)
        {
            copy_head = new_node;
            copy_current = copy_head;
        }
        else
        {
            copy_current->next = new_node;
            copy_current = copy_current->next;
        }
        current = current->next;
    }

    return copy_head;
}



char	*ft_realloc_string(char *str, int new_size)
{
	char	*res;
	int		old_len;

	if (!str)
		return (NULL);
	old_len = ft_strlen(str);
	res = malloc(old_len + new_size + 1);
	if (!res)
	{
		free(str);
		return (NULL);
	}
	ft_memcpy(res, str, old_len);
	res[old_len] = '\0';
	free(str);
	return (res);
}

char	*tab_to_str(char **tab)
{
	char	*str;
	int		i;

	str = malloc(1);
	i = -1;
	if (!str)
		return (NULL);
	str[0] = '\0';

	while (tab[++i])
	{
		str = ft_realloc_string(str, ft_strlen(tab[i]) + 2);
		if (!str)
			return (NULL);
		ft_strcat(str, tab[i]);
		if (tab[i + 1])
			ft_strcat(str, " ");
	}
	return (str);
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

void	free_split_line(char **split_line)
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
	t_token	*token_list;
	t_cmd	*cmd;
	int		len;
	//int		error;


	token_list = NULL;
	(void)argv;
	//error = 0;

	rl_outstream = stderr;

	signal(SIGPIPE, SIG_IGN);
	signals();
	cmd = NULL;
	len = -1;
	while (envp[++len]) {}
	char **tab = malloc(sizeof(char *) * (len + 1));
	cpy_tab(tab, envp);
	if (argc > 1)
	{
		g_signal = 0;
		return (check_error(ERROR_ARGS));
	}
	while (1)
	{
		if (malloc_structs(&cmd) != 0)
		{
			ft_putendl_fd(MALLOC_FAILURE, 2);
			g_signal = 1;
			return (0);
		}
		cmd->heredoc_processed = FALSE;
		cmd->env = tab;
		line = readline("$ ");
		split_line = ft_split_boosted(line);

		if (line == NULL)
		{
			ft_freetab(cmd->env);
			free_cmd(cmd);
			g_signal = 0;
			return (0);
		}
		add_history(line);
		if (space_only(line) == true)
		{
			free (line);
			continue;
		}
		if(ft_strcmp(split_line[0], "|") == 0)
		{
			printf("erreur synthaxe du au symbole ' | '\n");
			free_split_line(split_line);
			g_signal = 2;
			free(line);
			continue;
		}
		if (!check_pipe(split_line))
		{
			printf("erreur synthax symbole\n");
			g_signal = 2;
			free_split_line(split_line);

			//free(line);
			continue;
		}
		if (solo_quote(split_line) || badchar(split_line))
		{
			free_split_line(split_line);
			g_signal = 127;
			printf("synthax error : unmatching quote\n");
			// return (0);
			continue;
		}
		if (line == NULL)
		{
			//printf("je sors plutot la, checker leak de sortie\n");
			free_split_line(split_line);
			g_signal = 0;
			return (0);
		}
		if (!make_token(split_line, &token_list))
		{
			free_split_line(split_line);
			g_signal = 0;
			return (0);
		}


        cmd->tokens = copy_token_list(token_list);
        if (!cmd->tokens)
        {
            fprintf(stderr, "pas reussi a copier la liste de toekn\n");
            token_lstclear(&token_list, free);
            return (0);
        }

		// print_token_list(token_list);
		// printf("\n\n");
		path_main(token_list, envp);
		cmd->final_tab = main_cat(&token_list);//dedans ou celui dans dessou il y a un free token_list donc fais gaffe
		cmd->final_line = tab_to_str(cmd->final_tab);
		// printf("final_tab\n");
		// print_tab(cmd->final_tab);
		free(split_line);
		free(line);
		token_lstclear(&token_list, free);// si on le deplace c'est segfault assure
		process_input(cmd);
		token_lstclear(&cmd->tokens, free);// en test, a deplacer si ca foire
		free(cmd->final_line);
		tab = cmd->env;
	}
	//printf("sortie de loop\n");
	return (0);
}
