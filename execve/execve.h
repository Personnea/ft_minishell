/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:43:59 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/02 18:46:10 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECVE_H
# define EXECVE_H

# include "../libft/libft.h"
# include "../parser/parser.h"
# include "../envpath/envpath.h"
# include "../files_handler/fhandler.h"
# include "../terminal/program.h"
# include <sys/wait.h>

typedef struct s_commands
{
	t_parser			*cmd;
	char				**args;
	struct s_commands	*next;
}	t_commands;

char	*find_command(char *command, char *pathline);
void	clearmatrix(char **tab);
int		execve_handler(t_program *program);
void	do_command(t_parser *cmd, char *path, char **envp);
int		execve_with_pipe(t_program *program);
void	parse_commands_with_pipe(t_commands **commands, t_parser *parsed);


#endif