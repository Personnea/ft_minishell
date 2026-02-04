/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaigne <emaigne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 13:30:51 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/04 04:24:18 by emaigne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"

int	doing_here_doc_util(t_parser *lineread)
{
	int		fd;
	char	*line;

	fd = open(".here_doc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("here_doc: open"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strncmp(line, lineread->next->s,
				ft_strlen(lineread->next->s) + 1))
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	return (0);
}

int	doing_here_doc(t_parser **lineread)
{
	t_parser	*temp;

	temp = *lineread;
	while (temp)
	{
		if (temp->type == DELIMITER)
			doing_here_doc_util(temp);
		temp = temp->next;
	}
	return (0);
}
