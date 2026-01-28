/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:04:41 by abarthes          #+#    #+#             */
/*   Updated: 2026/01/28 18:22:32 by abarthes         ###   ########.fr       */
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
				//expand
				buildins(&test, envpath);
				//execve
			}
			free(line);
		}
	}
	return (0);
}