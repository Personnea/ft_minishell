/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fhandler.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 12:56:03 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/02 16:04:49 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FHANDLER_H
# define FHANDLER_H

# include "../parser/parser.h"
# include "../envpath/envpath.h"
# include <fcntl.h>


int			file_handler(t_parser **parsed);
t_parser	*get_last_output_file(t_parser **parsed);
t_parser	*get_last_input_file(t_parser **parsed);

#endif