/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:02:06 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/16 16:52:08 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int	restore_types_after_expansion(t_parser **tokens)
{
	t_parser	*cur;

	cur = *tokens;
	while (cur)
	{
		set_node_type(cur);
		cur = cur->next;
	}
	return (0);
}

int	add_empty_nodes_to_their_next(t_program *program)
{
	t_parser	*cur;
	t_parser	*to_remove;
	t_parser	*next;

	cur = *(program->parsed);
	while (cur)
	{
		to_remove = cur;
		next = cur->next;
		if (cur->s[0] == 0)
		{
			if (next)
				parser_clear_one(&to_remove, program);
		}
		cur = next;
	}
	return (0);
}