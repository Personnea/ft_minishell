/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:00:43 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/10 18:28:53 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "../libft/libft.h"
# include <stdio.h>

void	error_message_file_not_found(char *filename);
void	error_near_newline();
#endif