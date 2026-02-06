/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin_envpath.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 23:15:48 by emaigne           #+#    #+#             */
/*   Updated: 2026/02/06 18:56:03 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "buildins.h"

void static	free_array_of_double(char **array)
{
	free(array[0]);
	free(array[1]);
	free(array);
}

int	static	second_if_check(t_parser *cmd)
{
	if (cmd->next->s[0] == '=' || !ft_isalpha(cmd->next->s[0]))
		return (1);
	return (0);
}

int	buildin_export(t_parser *cmd, t_envpath *envpath)
{
	char	*key;
	char	*value;
	char	**array;

	if (!cmd->next)
		return (print_envpath_list_sorted(envpath));
	while (cmd->next && (cmd->next->type == CMD_ARG || cmd->next->type == CMD))
	{
		if (second_if_check(cmd))
			printf("export: %s: not a valid identifier\n", cmd->next->s);
		else
		{
			array = ft_split(cmd->next->s, '=');
			if (!array)
				return (1);
			key = ft_strdup(array[0]);
			if (!array[1] && ft_strchr(cmd->next->s, '='))
				value = ft_strdup("");
			else if (!array[1] && !ft_strchr(cmd->next->s, '='))
				value = 0;
			else
				value = ft_strdup(array[1]);
			free_array_of_double(array);
			if (!new_envpath(&envpath, key, value))
				return (1);
		}
		cmd = cmd->next;
	}
	return (0);
}

int	buildin_unset(t_parser *cmd, t_envpath *envpath)
{
	t_parser	*temp;
	t_envpath	*env_temp;

	temp = cmd->next;
	while (temp && temp->type == CMD_ARG)
	{
		env_temp = envpath;
		while (env_temp)
		{
			if (ft_strncmp(env_temp->index, temp->s, ft_strlen(temp->s)) == 0)
			{
				envp_delone(env_temp);
				break ;
			}
			env_temp = env_temp->next;
		}
		temp = temp->next;
	}
	return (0);
}
