/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:04:41 by abarthes          #+#    #+#             */
/*   Updated: 2026/01/28 16:00:48 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal.h"

int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	char	*line;
	t_envpath	*envpath;

	envpath = NULL;
	if (create_envpath_list(&envpath, envp) == 0)
		return (1);
	while (1)
	{
		line = readline("$miniswag>");
		if (!line)
				break ;
		if (line && *line)
		{
			add_history(line);
			t_parser *test = parsing(line);
			char *str;
			if (!test || !sanitize(&test))
				printf("syntax error\n");
			else
			{
				t_parser *temp = test;
				while (temp)
				{
					if (temp->type == CMD)
						str = "CMD";
					else if (temp->type == REDIR_OUTPUT)
						str = "REDIR_OUTPUT";
					else if (temp->type == FILENAME)
						str = "FILENAME";
					else if (temp->type == CMD_ARG)
						str = "CMD_ARG";
					else if (temp->type == ENVVAR)
						str = "ENVVAR";
					else if (temp->type == SQUOTE)
						str = "SQUOTE";
					else if (temp->type == DQUOTE)
						str = "DQUOTE";
					else if (temp->type == PIPE)
						str = "PIPE";
					else if (temp->type == EXIT_STATUS)
						str = "EXIT_STATUS";
					else if (temp->type == REDIR_INPUT)
						str = "REDIR_INPUT";
					else if (temp->type == REDIR_OUTPUT_APP)
						str = "REDIR_OUTPUT_APP";
					else
						str = "OTHER ?????";
					printf("Type: %s | Str: %s\n", str, temp->s);
					temp = temp->next;
				}
				temp = test;
				buildins(&temp, envpath);
			}
			free(line);
		}
	}
	return (0);
}