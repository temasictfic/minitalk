/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 03:06:23 by sciftci           #+#    #+#             */
/*   Updated: 2022/09/10 01:15:38 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk_bonus.h"

static void	client_handler(int sig)
{
	//if (sig == SIGUSR1)
		//ft_putstr_fd("\e[33m > ACK signal received from server\n\e[0m", 1);
	if (sig == SIGUSR2)
	{
		ft_putstr_fd("\e[92m > end of msg signal received from server\n\e[0m", 1);
		exit(EXIT_SUCCESS);
	}
}

static void	client_send_message(pid_t server_pid, char *str)
{
	size_t	i;
	size_t	strlen;

	strlen = ft_strlen(str);
	if (strlen != 0)
	{
		ft_putstr_fd("\e[92msending length = [", 1);
		ft_putnbr_fd(strlen, 1);
		ft_putstr_fd("]\n\e[0m", 1);
		send_len(server_pid, strlen);
		ft_putstr_fd("\e[92msending message\n\e[0m", 1);
		i = 0;
		while (str[i] != '\0')
			send_char(server_pid, str[i++]);
		ft_putstr_fd("\e[92msending null string terminator\n\e[0m", 1);
		send_char(server_pid, '\0');
	}
}

int	main(int argc, char **argv)
{
	struct sigaction	s_client;
	pid_t				pid;

	if (argc != 3)
	{
		ft_putstr_fd("\e[31m## error - incorrect syntax ##\n\e[0m", 1);
		ft_putstr_fd("\e[92m./client <server PID> <string to send>\n\e[0m", 1);
		return (EXIT_FAILURE);
	}
	pid = ft_atoi(argv[1]);
	if (kill(pid, 0) < 0)
	{
		ft_putstr_fd("\e[31m## error - PID is invalid ##\n\e[0m", 1);
		return (EXIT_FAILURE);
	}
	sigemptyset(&s_client.sa_mask);
	s_client.sa_flags = SA_RESTART | SA_SIGINFO;
	s_client.sa_handler = client_handler;
	check_sigaction_signals(&s_client);
	ft_putstr_fd("\e[92mclient [PID = ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putstr_fd("]\n\e[0m", 1);
	client_send_message(pid, argv[2]);
	return (EXIT_SUCCESS);
}
