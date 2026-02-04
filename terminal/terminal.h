/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaigne <emaigne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:04:34 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/04 03:22:02 by emaigne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINAL_H
# define TERMINAL_H

# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "../parser/parser.h"
# include "../buildins/buildins.h"
# include "../envpath/envpath.h"
# include "../expand/expand.h"
# include "../signals/signals.h"
# include "../files_handler/fhandler.h"
# include "../here_doc/here_doc.h"
# include "../execve/execve.h"

# ifndef IS_DEBUG
#  define IS_DEBUG 0
# endif

//					---DEBUG---				//
void		print_debug(t_program *program);

//					---Handlers---			//
void		handle_redirections(t_program *program);
void		handle_expansions(t_program *program);
void		execute_and_restore(t_program *program);

#endif