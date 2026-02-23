/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaigne <emaigne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:53:31 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/23 17:53:06 by emaigne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execve.h"
#include "../here_doc/here_doc.h"

void	exit_is_dir(t_program *program, t_commands *cmd, char **args)
{
	error_message_is_a_directory(cmd->cmd->s);
	free_t_program(program);
	clearmatrix(args);
	exit(126);
}

void	exit_check_args_cmd(t_program *program, char **args,
	char *new_cmd, t_commands *cmd)
{
	if (!args)
	{
		free_t_program(program);
		clearmatrix(args);
		exit (1);
	}
	if (!new_cmd)
	{
		error_message_command_not_found(cmd->cmd->s);
		free_t_program(program);
		clearmatrix(args);
		exit(127);
	}
}

void	do_command_piped(t_program *program, t_commands *cmd,
	char *path, char **envp)
{
	char		*new_cmd;
	char		**args;
	struct stat	path_stat;

	if (cmd->cmd->type == DELIMITER)
	{
		free_t_command(cmd);
		free_t_program(program);
		exit(1);
	}
	new_cmd = find_command(cmd->cmd->s, path);
	args = ft_dup_matrix(cmd->args);
	exit_check_args_cmd(program, args, new_cmd, cmd);
	if (stat(new_cmd, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		exit_is_dir(program, cmd, args);
	tcsetattr(STDIN_FILENO, TCSANOW, &program->g_term_orig);
	free_t_program(program);
	execve(new_cmd, args, envp);
	error_message_command_not_found(cmd->cmd->s);
	free(new_cmd);
	free(args);
	exit(127);
}

void	do_command(t_program *program, t_parser *cmd, char *path, char **envp)
{
	char		**splited_cmd;
	char		*new_cmd;
	int			i;
	struct stat	path_stat;

	i = 1;
	splited_cmd = malloc(sizeof(char *) * (count_cmd_args(cmd) + 2));
	if (!splited_cmd)
		exit(1);
	splited_cmd = provision_commands(cmd, splited_cmd);
	if (!splited_cmd)
		clean_exit(program, NULL, NULL, 127);
	new_cmd = find_command(cmd->s, path);
	if (!new_cmd)
		clean_exit(program, splited_cmd, new_cmd, 127);
	if (stat(new_cmd, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		error_message_is_a_directory(cmd->s);
		clean_exit(program, splited_cmd, new_cmd, 126);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &program->g_term_orig);
	close (program->saved_stdin);
	close (program->saved_stdout);
	execve(new_cmd, splited_cmd, envp);
	clean_exit(program, splited_cmd, new_cmd, 127);
}
