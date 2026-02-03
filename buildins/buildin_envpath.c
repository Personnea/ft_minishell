/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin_envpath.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaigne <emaigne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 23:15:48 by emaigne           #+#    #+#             */
/*   Updated: 2026/02/03 01:22:39 by emaigne          ###   ########.fr       */
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
	if ((!cmd->next || cmd->next->type != CMD_ARG
			|| !ft_strchr(cmd->next->s, '=') || cmd->next->s[0] == '=')
		|| !ft_isalpha(cmd->next->s[0]))
		return (1);
	return (0);
}

int	static	third_if_check(t_parser *cmd)
{
	return (cmd->next->next && (cmd->next->next->type == CMD_ARG
			|| cmd->next->next->type == CMD));
}

int	buildin_export(t_parser *cmd, t_envpath *envpath)
{
	char	*key;
	char	*value;
	char	**array;

	if (!cmd->next)
		return (print_envpath_list_sorted(envpath));
	if (second_if_check(cmd))
		return (1);
	if (third_if_check(cmd))
	{
		key = ft_strtrim(cmd->next->s, "=");
		value = cmd->next->next->s;
	}
	else
	{
		array = ft_split(cmd->next->s, '=');
		printf("Split result: %s, %s\n", array[0], array[1]);
		if (!array || !array[0] || !array[1])
			return (1);
		key = ft_strdup(array[0]);
		value = ft_strdup(array[1]);
		free_array_of_double(array);
	}
	printf("Exporting key: %s with value: %s\n", key, value);
	return (new_envpath(&envpath, key, value));
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
