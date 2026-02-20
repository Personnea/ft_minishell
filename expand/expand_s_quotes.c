/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_s_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaigne <emaigne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 07:00:39 by emaigne           #+#    #+#             */
/*   Updated: 2026/02/20 07:00:55 by emaigne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int	expand_s_quote(t_parser **node, t_program *program)
{
	char	*new_str;
	int		len;

	len = ft_strlen((*node)->s);
	new_str = malloc((len - 1) * sizeof(char));
	if (!new_str)
		return (1);
	ft_strlcpy(new_str, (*node)->s + 1, len - 1);
	free((*node)->s);
	(*node)->s = new_str;
	if ((*node)->s[0] == 0)
		return (parser_clear_one(node, program), 0);
	(*node)->type = WAS_EXPANDED;
	return (0);
}
