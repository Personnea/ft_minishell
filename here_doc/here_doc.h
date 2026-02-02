/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 13:29:58 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/02 13:49:50 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOC_H
# define HERE_DOC_H

# include "../libft/libft.h"
# include "../parser/parser.h"
# include <fcntl.h>
# include <readline/readline.h>

int	doing_here_doc(t_parser **lineread);

#endif