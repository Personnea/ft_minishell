/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaigne <emaigne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 15:48:39 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/03 02:33:17 by emaigne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_parser	*get_last_parser(t_parser *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

t_parser	*get_first_parser(t_parser *lst)
{
	if (!lst)
		return (0);
	while (lst->prev)
		lst = lst->prev;
	return (lst);
}

void	parser_clear_one(t_parser *node)
{
	if (!node)
		return ;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	free(node->s);
	free(node);
}

int	new_parser(t_parser **head, t_parser *new_node)
{
	if (!new_node)
		return (0);
	parser_add_back(head, new_node);
	return (1);
}
