/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 03:29:44 by sciftci           #+#    #+#             */
/*   Updated: 2022/09/08 18:04:15 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk.h"

/*
  Function checks if the string message is totaly received.
  In case the null terminator string is received, prints the whole string
  and frees the string from heap memory and informs client with send_bit() 
  that message was received (client will then exit) without signal expecting 
  to be received back from client
*/
static void	msg_received(t_protocol *t_server, pid_t client_pid)
{
	if (t_server->bits == 8 && t_server->flag == 1)
	{
		t_server->message[t_server->index] = t_server->data;
		t_server->index++;
		if (t_server->data == '\0')
		{
			ft_putstr_fd("\e[92mreceived message = [", 1);
			ft_putstr_fd(t_server->message, 1);
			ft_putstr_fd("]\n\e[0m", 1);
			free(t_server->message);
			t_server->message = NULL;
			t_server->flag = 0;
			t_server->index = 0;
			send_bit(client_pid, 1, 0);
		}
		t_server->bits = 0;
	}
}

/*
  Function checks if the string length bits are done
  If yes, the length is printed to stdout and used to allocated in the 
  heap memory a string with the exact size received (plus the null terminator)
*/
static void	msg_len_received(t_protocol *t_server)
{
	if (t_server->bits == sizeof(int) * 8 && t_server->flag == 0)
	{
		t_server->flag = 1;
		ft_putstr_fd("\e[92mreceived length = [", 1);
		ft_putnbr_fd(t_server->data, 1);
		ft_putstr_fd("]\n\e[0m", 1);
		t_server->message = malloc((t_server->data + 1) * sizeof(char));
		if (t_server->message == NULL)
		{
			ft_putstr_fd("\e[31m## error - malloc() ##\n\e[0m", 1);
			exit(EXIT_FAILURE);
		}
		t_server->message[t_server->data] = '\0';
		t_server->bits = 0;
	}
}

/*
  Function catches SIGUSR1 and SIGUSR2 signals received from client that
  represents the status of each bit (either 0 or 1)
  Variables declared as static are initialized with zero, and after each 
  8 bits received they are again re-initialized to zero
  For each bit received from client, the server sends an ACK signal
  (then the client should only send the next bit after this ACK signal)
*/
static void	server_handler(int sig, siginfo_t *info, void *context)
{
	static t_protocol	t_server;

	usleep(WAIT_TIME);
	(void)context;
	if (t_server.bits == 0)
		t_server.data = 0;
	if (sig == SIGUSR2 && t_server.flag == 0)
		t_server.data |= 1 << (((sizeof(int) * 8) - 1) - t_server.bits);
	else if (sig == SIGUSR2 && t_server.flag == 1)
		t_server.data |= 1 << (((sizeof(char) * 8) - 1) - t_server.bits);
	t_server.bits++;
	msg_len_received(&t_server);
	msg_received(&t_server, info->si_pid);
	send_bit(info->si_pid, 0, 0);
}

/*
  This program (server) prints to stdout the PID process and keeps
  listening for incoming message transmissions
  
  Each client should use the following sequence:
         (int)              (char)               (char)
    length of message -> regular chars -> null string terminator char
  The string message is only printed to stdout when the full string is received
  For each signal received from client, this program (server) sends
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
	s_server.sa_flags = SA_SIGINFO | SA_RESTART;
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
