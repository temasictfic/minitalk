/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 01:56:12 by sciftci           #+#    #+#             */
/*   Updated: 2022/09/11 19:52:03 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk.h"

/*
  Function sets the signals to be caught by the custom handler.
  In case this action fails, prints to stdout an error message and exits the
  program
*/
void	check_sigaction_signals(struct sigaction *sa)
{
	if (sigaction(SIGUSR1, sa, NULL) < 0)
	{
		ft_putstr_fd("\e[31m## error - could not setup SIGUSR1 ##\n\e[0m", 1);
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGUSR2, sa, NULL) < 0)
	{
		ft_putstr_fd("\e[31m## error - could not setup SIGUSR2 ##\n\e[0m", 1);
		exit(EXIT_FAILURE);
	}
}

/*
  Functions sends an integer containing the length of the message
  For each bit sent client, waits a signal received back before proceeding
  by using flag = 1 on the send_bit()
  Assumed 1 byte = 8 bits
*/
void	send_char(pid_t pid, char c)
{
	int		shift;
	char	bit;

	shift = (sizeof(char) * 8) - 1;
	while (shift >= 0)
	{
		bit = (c >> shift) & 1;
		send_bit(pid, bit, 1);
		shift--;
	}
}

/*
  Function sends a bit (0 or 1) to the process PID
  Return from function will happen after ack signal is received in case
  the pause flag is set to non zero, otherwise return immediately
*/
void	send_bit(pid_t pid, char bit, char pause_flag)
{
	if (bit == 0)
	{
		if (kill(pid, SIGUSR1) < 0)
		{
			ft_putstr_fd("\e[31m## error - sending SIGUSR1 ##\n\e[0m", 1);
			exit(EXIT_FAILURE);
		}
	}
	else if (bit == 1)
	{
		if (kill(pid, SIGUSR2) < 0)
		{
			ft_putstr_fd("\e[31m## error - sending SIGUSR2 ##\n\e[0m", 1);
			exit(EXIT_FAILURE);
		}
	}
	if (pause_flag != 0)
		pause();
}
