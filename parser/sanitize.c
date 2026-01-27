/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 14:53:12 by abarthes          #+#    #+#             */
/*   Updated: 2026/01/27 15:31:09 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	sanitize(t_parser **head)
{
	t_parser	*temp;

	temp = *head;
	while (temp)
	{
		if (temp->type == PIPE)
		{
			if (!temp->next || !temp->prev || temp == *head || !(temp->next->type == CMD) || !(temp->prev->type == CMD || temp->prev->type == CMD_ARG))
				return (0);
		}
		if (temp->type == REDIR_INPUT)
		{
			if (!temp->next || !(temp->next->type == FILENAME))
				return (0);
		}
		if (temp->type == REDIR_OUTPUT || temp->type == REDIR_OUTPUT_APP)
		{
			if (!temp->next || !(temp->next->type == FILENAME))
				return (0);
		}
		temp = temp->next;
	}
	return (1);
}

//pipe not at start or end between 2 cmds
//redir must be followed by filename