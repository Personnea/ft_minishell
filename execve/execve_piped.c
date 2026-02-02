/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_piped.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 17:16:41 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/02 19:33:07 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execve.h"

void	do_command_piped(t_commands *cmd, char *path, char **envp)
{
	char		*new_cmd;

	new_cmd = find_command(cmd->cmd->s, path);
	if (!new_cmd)
		exit (1);
	execve(new_cmd, cmd->args, envp);
	perror("execve:");
	free(new_cmd);
	exit(1);
}

void	last_exec(t_program *program, t_commands *cmd)
{
	pid_t	pid;
	int		fd;
	char	*path;
	t_parser 	*file;

	pid = fork();
	if (pid == -1)
		return (perror("pid"), exit(1));
	if (pid)
		return ;
	else
	{
		file = get_last_output_file(program->parsed);
		if (!file)
			fd = program->saved_stdout;
		else
			fd = open(file->s, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open");
			exit(1);
		}
		if (fd == 0)
			fd = program->saved_stdout;
		dup2(fd, STDOUT_FILENO);
		close(fd);
		path = get_env_value_by_key(*(program->envpath), "PATH");
		do_command_piped(cmd, path, program->envp);
	}
}

void	middle_exec(t_program *program, t_commands *cmd)
{
	pid_t	pid;
	int		pipe_fd[2];
	char	*path;

	if (pipe(pipe_fd) != 0)
		return (perror("pipe"), exit (1));
	pid = fork();
	if (pid == -1)
		return (perror("pid"), close(pipe_fd[0]), close(pipe_fd[1]),
			exit(1));
	if (pid)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
	else
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		path = get_env_value_by_key(*(program->envpath), "PATH");
		do_command_piped(cmd, path, program->envp);
	}
}

void	handle_the_child(int pipe_fd[2], t_program *program, t_commands *cmd)
{
	int			fd;
	char		*path;
	t_parser 	*file;

	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	file = get_last_input_file(program->parsed);
	if (!file)
		fd = program->saved_stdin;
	else
		fd = open(file->s, O_RDONLY);
	if (fd < 0)
		return (perror("open"), exit(1));
	dup2(fd, STDIN_FILENO);
	close(fd);
	path = get_env_value_by_key(*(program->envpath), "PATH");
	do_command_piped(cmd, path, program->envp);
}

void	first_exec(t_program *program, t_commands *cmd)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (pipe(pipe_fd) != 0)
		return (perror("pipe"), exit (1));
	pid = fork();
	if (pid == -1)
		return (perror("pid"), close(pipe_fd[0]), close(pipe_fd[1]),
			exit(1));
	if (pid)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
	else
	{
		handle_the_child(pipe_fd, program, cmd);
	}
}


int	execve_with_pipe(t_program *program)
{
	int			status;
	t_commands	*commands;

	commands = NULL;
	status = 0;
	parse_commands_with_pipe(&commands, *(program->parsed));
	first_exec(program, commands);
	commands = commands->next;
	while (commands && commands->next)
	{
		middle_exec(program, commands);
		commands = commands->next;
	}
	last_exec(program, commands);
	waitpid(-1, &status, 0);
	program->last_exit_status = status;
	return (status);
}
