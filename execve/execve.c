/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthes <abarthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:44:15 by abarthes          #+#    #+#             */
/*   Updated: 2026/02/20 15:06:30 by abarthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execve.h"
#include "../here_doc/here_doc.h"
#include "../signals/signals.h"
#include <signal.h>

int	execve_handler(t_program *program)
{
	int					status;
	int					last_status;
	struct sigaction	sa;
	struct sigaction	old_int;
	struct sigaction	old_quit;
	int				already_n;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &old_int);
	sigaction(SIGQUIT, &sa, &old_quit);
	already_n = 0;
	if (there_is_at_least_one_pipe(*(program->parsed)))
	{
		if (execve_with_pipe(program))
			return (sigaction(SIGINT, &old_int, NULL), 
				sigaction(SIGQUIT, &old_quit, NULL), 1);
	}
	else
	{
		if (execve_without_pipe(program, program->parsed,
				*(program->envpath), program->envp))
			return (sigaction(SIGINT, &old_int, NULL), 
				sigaction(SIGQUIT, &old_quit, NULL), 1);
	}
	last_status = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFSIGNALED(status) && !already_n)
		{
			write(STDOUT_FILENO, "\n", 1);
			already_n = 1;
		}
		last_status = WEXITSTATUS(status);
	}
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	tcsetattr(STDIN_FILENO, TCSANOW, &program->g_term_orig);
	program->last_exit_status = last_status;
	return (last_status);
}
