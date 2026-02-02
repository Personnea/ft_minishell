/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:44:15 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/02 19:33:13 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execve.h"

int	is_a_buildin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4) == 0 && ft_strlen(cmd) == 4)
		return (1);
	else if (ft_strncmp(cmd, "cd", 2) == 0 && ft_strlen(cmd) == 2)
		return (1);
	else if (ft_strncmp(cmd, "pwd", 3) == 0 && ft_strlen(cmd) == 3)
		return (1);
	else if (ft_strncmp(cmd, "export", 6) == 0 && ft_strlen(cmd) == 6)
		return (1);
	else if (ft_strncmp(cmd, "unset", 5) == 0 && ft_strlen(cmd) == 5)
		return (1);
	else if (ft_strncmp(cmd, "env", 3) == 0 && ft_strlen(cmd) == 3)
		return (1);
	else if (ft_strncmp(cmd, "exit", 4) == 0 && ft_strlen(cmd) == 4)
		return (1);
	return (0);
}

t_parser	*get_first_cmd_no_buildins(t_parser *cmd)
{
	t_parser	*temp;

	temp = cmd;
	while (temp)
	{
		if (temp->type == CMD && !is_a_buildin(temp->s))
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

int	count_cmd_args(t_parser *cmd)
{
	t_parser	*temp;
	int			count;

	count = 0;
	temp = cmd->next;
	while (temp && temp->type == CMD_ARG)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

void	do_command(t_parser *cmd, char *path, char **envp)
{
	char		**splited_cmd;
	char		*new_cmd;
	t_parser	*temp;
	int			i;

	i = 1;
	splited_cmd = malloc(sizeof(char *) * (count_cmd_args(cmd) + 2));
	splited_cmd[0] = cmd->s;
	temp = cmd->next;
	while (temp && temp->type == CMD_ARG)
	{
		splited_cmd[i++] = temp->s;
		temp = temp->next;
	}
	splited_cmd[i] = NULL;
	new_cmd = find_command(cmd->s, path);
	if (!new_cmd)
	{
		clearmatrix(splited_cmd);
		exit (1);
	}
	execve(new_cmd, splited_cmd, envp);
	perror("execve:");
	clearmatrix(splited_cmd);
	free(new_cmd);
	exit(1);
}

void	exec_one_command(t_parser *cmd, char *path, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("pid"), exit(1));
	if (pid)
		return ;
	else
		do_command(cmd, path, envp);
}

int	make_redirection(t_parser *parsed)
{
	t_parser	*last_file_output;
	t_parser	*last_file_input;
	int			fd;

	last_file_output = get_last_output_file(&parsed);
	last_file_input = get_last_input_file(&parsed);
	if (last_file_output)
	{
		fd = open(last_file_output->s, O_WRONLY);
		if (fd < 0)
		{
			char src[1024] = "minishell: ";
			ft_strlcat(src, last_file_output->s, 1010);
			perror(src);
			return (1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (last_file_input)
	{
		fd = open(last_file_input->s, O_RDONLY);
		if (fd < 0)
		{
			char src[1024] = "minishell: ";
			ft_strlcat(src, last_file_input->s, 1010);
			perror(src);
			return (1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

int	execve_without_pipe(t_parser **parsed, t_envpath *envpath, char **envp)
{
	t_parser	*cmd;

	cmd = get_first_cmd_no_buildins(*parsed);
	if (!cmd)
		return (1);
	make_redirection(*parsed);
	exec_one_command(cmd, get_env_value_by_key(envpath, "PATH"), envp);
	return (0);
}

int	there_is_at_least_one_pipe(t_parser *lineread)
{
	t_parser	*temp;

	temp = lineread;
	while (temp)
	{
		if (temp->type == PIPE)
			return (1);
		temp = temp->next;
	}
	return (0);
}

// int	execve_handler(t_parser **lineread, t_envpath *envpath, char **envp)
int	execve_handler(t_program *program)
{
	int	status;

	status = 0;
	if (there_is_at_least_one_pipe(*(program->parsed)))
	{
		if (execve_with_pipe(program))
			return (1);
	}
	else
	{
		if (execve_without_pipe(program->parsed, *(program->envpath), program->envp))
			return (1);
	}
	waitpid(-1, &status, 0);
	program->last_exit_status = status;
	return (status);
}
