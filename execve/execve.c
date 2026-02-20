/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:44:15 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/20 15:33:56 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execve.h"
#include "../here_doc/here_doc.h"
#include "../signals/signals.h"
#include <signal.h>

int	execve_maker(t_program *program)
{
	t_signal_handlers	handlers;

	handlers = setup_signals_before_fork();
	if (there_is_at_least_one_pipe(*(program->parsed)))
	{
		if (execve_with_pipe(program))
			return (restore_signals(handlers), 1);
	}
	else
	{
		if (execve_without_pipe(program, program->parsed,
				*(program->envpath), program->envp))
			return (restore_signals(handlers), 1);
	}
	return (restore_signals(handlers), 0);
}

int	execve_handler(t_program *program)
{
	int					status;
	int					last_status;
	int					already_n;
	t_signal_handlers	handlers;

	handlers = setup_signals_before_fork();
	already_n = 0;
	if (execve_maker(program))
		return (1);
	last_status = 0;
	setup_signals_after_fork();
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFSIGNALED(status) && !already_n)
		{
			write(STDOUT_FILENO, "\n", 1);
			already_n = 1;
		}
		last_status = WEXITSTATUS(status);
	}
	restore_signals(handlers);
	tcsetattr(STDIN_FILENO, TCSANOW, &program->g_term_orig);
	program->last_exit_status = last_status;
	return (last_status);
}
