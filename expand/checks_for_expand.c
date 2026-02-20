/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_for_expand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaigne <emaigne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 07:08:50 by emaigne           #+#    #+#             */
/*   Updated: 2026/02/20 07:09:41 by emaigne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int check_and_count_for_envvar(t_parser *n, t_envpath *ep)
{
	int		i;
	int		j;
	int		t_count;
	char	*key;
	char	*value;

	i = -1;
	t_count = 0;
	while (n->s[++i])
	{
		if (n->s[i] == '$' && ft_isalnum(n->s[i+1]))
		{
			j = i + 1;
			while (n->s[j] && ft_isalnum(n->s[j]))
				j++; 
			key = ft_substr(n->s, i + 1, j - i - 1);
			if (!key)
				return (-1);
			value = get_env_value_by_key(&ep, key);
			if (value)
				t_count += ft_strlen(value);
			i = j - 1;
			free(key);
		}
		else
			t_count++;
	}
	return (t_count);
}

int	is_env_var(t_parser *node, int i)
{
	return (node->s[i] == '$'
		&& (ft_isalnum(node->s[i + 1]) || node->s[i + 1] == '_'));
}
