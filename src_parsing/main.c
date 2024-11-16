/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdelamar <cdelamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 13:11:04 by laubry            #+#    #+#             */
/*   Updated: 2024/11/16 01:29:09 by laubry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**split_line;
	t_token	*token_list;
	t_cmd	*cmd;
	int		len;

	token_list = NULL;
	(void)argv;
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
			return (1);
		}
		cmd->heredoc_processed = FALSE;
		cmd->env = tab;
		line = readline("$ ");
		split_line = ft_split_boosted(line);
		if (line == NULL)
		{
			ft_freetab(cmd->env);
			free_cmd(cmd);
			return (g_signal);
		}

		add_history(line);
		if (space_only(line) == true)
		{
			free (line);
			continue;
		}
		if(ft_strcmp(split_line[0], "|") == 0)
		{
			printf("synthax error near \"|\"\n");
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
			continue;
		}
		if (solo_quote(split_line) || badchar(split_line) || qte(split_line))
		{
			free_split_line(split_line);
			g_signal = 127;
			printf("synthax error : unmatching quote\n");
			continue;
		}
		if (line == NULL)
		{
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
		free(split_line);
		path_main(&token_list, tab);
		if (token_list == NULL)
			continue ;
        cmd->tokens = copy_token_list(token_list);
        if (!cmd->tokens)
        {
            fprintf(stderr, "pas reussi a copier la liste de toekn\n");
            token_lstclear(&token_list, free);
            return (0);
        }
		cmd->final_tab = main_cat(&token_list);
		cmd->final_line = tab_to_str(cmd->final_tab);
		free(line);
		token_lstclear(&token_list, free);
		process_input(cmd);
		token_lstclear(&cmd->tokens, free);
		free(cmd->final_line);
		tab = cmd->env;
	}
	return (0);
}
