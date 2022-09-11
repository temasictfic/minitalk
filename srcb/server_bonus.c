/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 04:10:32 by sciftci           #+#    #+#             */
/*   Updated: 2022/09/11 20:20:23 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk_bonus.h"

/*
  Function checks if the string message is totaly received.
  prints message char by char and informs client with send_bit() 
  client will then exit without signal expecting 
  to be received back from client
*/
static void	msg_received(t_protocol *t_server, pid_t client_pid)
{
	if (t_server->bits == 8)
	{
		ft_putchar_fd(t_server->data, 1);
		if (t_server->data == '\0')
		{
			ft_putstr_fd("\n\e[92m/*\\received message/*\\\n\e[0m", 1);
			send_bit(client_pid, 1, 0);
		}
		t_server->bits = 0;
	}
}

/*
  Function catches SIGUSR1 and SIGUSR2 signals received from client that
  represents the status of each bit (either 0 or 1)
  Variables declared as static are initialized with zero, and after each 
  8 bits received they are again re-initialized to zero
  For each bit received from client, the server sends an ack signal
  (then the client should only send the next bit after this ack signal)
*/
static void	server_handler(int sig, siginfo_t *info, void *context)
{
	static t_protocol	t_server;
	static pid_t		client_pid;

	usleep(WAIT_TIME);
	if (info->si_pid != 0)
		client_pid = info->si_pid;
	(void)context;
	if (t_server.bits == 0)
		t_server.data = 0;
	if (sig == SIGUSR2)
		t_server.data |= 1 << (((sizeof(char) * 8) - 1) - t_server.bits);
	t_server.bits++;
	msg_received(&t_server, client_pid);
	send_bit(client_pid, 0, 0);
}

/*
  This program (server) prints to stdout the PID process and keeps
  listening for incoming message transmissions
  
  The string message is printed to stdout char by char when is received
  For each message received from client, this program (server) sends
  a corresponding signal back
  
  Using sigaction structure to define which signals to catch by the handler
  The infinite loop will just keep the process running
  
  The pause() function causes the calling thread to pause until a
  signal is received
*/
int	main(void)
{
	struct sigaction	s_server;

	sigemptyset(&s_server.sa_mask);
	s_server.sa_sigaction = server_handler;
	s_server.sa_flags = SA_SIGINFO;
	check_sigaction_signals(&s_server);
	ft_putstr_fd("\e[92mserver [PID = ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putstr_fd("]\n\e[0m", 1);
	while (1)
	{
		pause();
	}
	return (EXIT_SUCCESS);
}
