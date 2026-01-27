/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 16:41:26 by abarthes          #+#    #+#             */
/*   Updated: 2026/01/27 17:59:14 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "buildins.h"

int buildin_echo(t_parser *cmd)
{
	t_parser	*temp;
	int			is_n;

	temp = cmd->next;
	if (ft_strncmp(temp->s, "-n", 2) == 0)
	{
		is_n = 1;
		temp = temp->next;
	}
	while (temp && temp->type == CMD_ARG)
	{
		ft_printf("%s", temp->s);
		temp = temp->next;
		if (temp && temp->type == CMD_ARG)
			ft_printf(" ");
	}
	if (!is_n)
		ft_printf("\n");
	return (0);
}

int	buildin_exit(void)
{
	exit(0);
	return (0);
}

int	buildin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("getcwd() error");
		return (1);
	}
}

int	buildin_cd(t_parser *cmd)
{
	if (!cmd->next || cmd->next->type != CMD_ARG)
	{
		if (chdir("../") != 0)
			return (perror("cd"), 1);
		return (0);
	}
	else
	{
		if (chdir(cmd->next->s) != 0)
			return (perror("cd"), 1);
		return (0);
	}
}

int	check_buildin(t_parser *cmd)
{
	if (cmd->type == CMD && ft_strncmp(cmd->s, "cd", 2) == 0)
		return (buildin_cd(cmd));
	else if (cmd->type == CMD && ft_strncmp(cmd->s, "echo", 4) == 0)
		return (buildin_echo(cmd));
	else if (cmd->type == CMD && ft_strncmp(cmd->s, "exit", 4) == 0)
		return (buildin_exit());
	else if (cmd->type == CMD && ft_strncmp(cmd->s, "export", 6) == 0)
		printf("buildin export\n");
	else if (cmd->type == CMD && ft_strncmp(cmd->s, "unset", 5) == 0)
		printf("buildin unset\n");
	else if (cmd->type == CMD && ft_strncmp(cmd->s, "pwd", 3) == 0)
		return (buildin_pwd());
	else if (cmd->type == CMD && ft_strncmp(cmd->s, "env", 3) == 0)
		printf("buildin env\n");
	return (1);
}

int	buildins(t_parser **parser)
{
	t_parser	*temp;

	temp = *parser;
	while (temp)
	{
		if (check_buildin(temp))
			return (1);
		temp = temp->next;
	}
	return (0);
}
