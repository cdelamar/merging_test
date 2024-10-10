#include "../includes/minishell.h"


int count_commands(t_cmd *cmd)
{
    int i = 0;
    int count = 1;

    while (cmd->final_tab[i])
    {
        if (strcmp(cmd->final_tab[i], "|") == 0)
            count++;
        i++;
    }
    return count;
}

char **split_commands(t_cmd *cmd)
{
    int i = 0;
    int j = 0;
    char **commands = malloc((count_commands(cmd) + 1) * sizeof(char *));
    char *temp = NULL;

    while (cmd->final_tab[i])
    {
        if (strcmp(cmd->final_tab[i], "|") == 0)
        {
            commands[j++] = temp;
            temp = NULL;
        }
        else
        {
            if (!temp)
                temp = strdup(cmd->final_tab[i]);
            else
            {
                char *tmp = malloc(strlen(temp) + strlen(cmd->final_tab[i]) + 2);
                strcpy(tmp, temp);
                strcat(tmp, " ");
                strcat(tmp, cmd->final_tab[i]);
                free(temp);
                temp = tmp;
            }
        }
        i++;
    }
    commands[j++] = temp;
    commands[j] = NULL;
    return commands;
}

void ft_path_command(t_cmd *cmd)
{
    cmd->fd_in = 0; // Initialize the input for the first commad
    cmd->path_command = split_commands(cmd);
	//printf ("CREATED path_command\n");
	// print_tab(cmd->path_command);
}