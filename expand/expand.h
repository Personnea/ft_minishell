/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:49:19 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/02 19:36:30 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "../parser/parser.h"
# include "../terminal/terminal.h"
# include "../envpath/envpath.h"
# include "../terminal/program.h"

int	send_to_expand(t_parser **parsed, t_envpath *envpath, t_program *program);

#endif