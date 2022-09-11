/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 03:06:23 by sciftci           #+#    #+#             */
/*   Updated: 2022/09/11 19:56:50 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk_bonus.h"

/*
  Function handles the SIGUSR1/SIGUSR2 signals received from server
  
  To be noted that the server will only send a signal upon receiving a 
  signal from server, as explained below:
  - Client sends a bit to server, and (normally) waits a reply - using pause()
  and then
  - The server sends a signal to confirm that each bit was received (ack)
  or
  - The server sends a signal to confirm that the NULL terminator was received
  (end of string message), and then the function exits
  
  else if (sig == SIGUSR1)
	ft_putstr_fd("\e[33m > beat signal received from server\n\e[0m", 1);
*/
static void	client_handler(int sig)
{
	if (sig == SIGUSR2)
	{
		ft_putstr_fd("\e[92m#end of msg signal received from server\n\e[0m", 1);
		exit(EXIT_SUCCESS);
	}
}

/*
  Function sends the string itself, including the null terminator
*/
static void	client_send_message(pid_t server_pid, char *str)
{
	size_t	i;

	i = 0;
	ft_putstr_fd("\e[92msending message\n\e[0m", 1);
	while (str[i] != '\0')
		send_char(server_pid, str[i++]);
	ft_putstr_fd("\e[92msending null string terminator\n\e[0m", 1);
	send_char(server_pid, '\0');
}

/*
  This program (client) sends a string message to another process (server)
  The arguments passed from command line are checked if valid
  - string and PID server arguments must be passed, and not more or less
  - PID server must be valid, kill() and ft_atoi() are used to check this
  (not only it must be a valid number as it must be an existing process)
  The sigaction structure is used to catch the user defined signals and 
  take care of the communication with server. Basically for each signal
  sent from client, a reply signal is expected to be received from server
*/
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
	s_client.sa_flags = SA_SIGINFO;
	s_client.sa_handler = client_handler;
	check_sigaction_signals(&s_client);
	ft_putstr_fd("\e[92mclient [PID = ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putstr_fd("]\n\e[0m", 1);
	client_send_message(pid, argv[2]);
	return (EXIT_SUCCESS);
}
