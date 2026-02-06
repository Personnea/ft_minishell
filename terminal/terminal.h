/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:04:34 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/06 12:29:32 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINAL_H
# define TERMINAL_H

# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

# if defined(__APPLE__)
/* On some macOS setups the readline header (or libedit compatibility) may
	not declare these functions; provide minimal prototypes to avoid
	implicit-declaration errors at compile time. */
void rl_replace_line(const char *text, int clear_undo);
void rl_redisplay(void);
# endif
# include "../parser/parser.h"
# include "../buildins/buildins.h"
# include "../envpath/envpath.h"
# include "../expand/expand.h"
# include "../signals/signals.h"
# include "../files_handler/fhandler.h"
# include "../here_doc/here_doc.h"
# include "../execve/execve.h"

#endif