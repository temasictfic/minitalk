/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 04:10:32 by sciftci           #+#    #+#             */
/*   Updated: 2022/08/23 15:59:37 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk_bonus.h"

static void	msg_received(t_protocol *t_server, size_t *i, pid_t client_pid)
{
	if (t_server->bits == 8 && t_server->flag == 1)
	{
		t_server->message[*i] = t_server->data;
		(*i)++;
		if (t_server->data == '\0')
		{
			ft_putstr_fd("\e[92mreceived message = [", 1);
			ft_putstr_fd(t_server->message, 1);
			ft_putstr_fd("]\n\e[0m", 1);
			free(t_server->message);
			t_server->message = NULL;
			t_server->flag = 0;
			*i = 0;
			send_bit(client_pid, 1, 0);
		}
		t_server->bits = 0;
	}
}

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

static void	server_handler(int sig, siginfo_t *info, void *context)
{
	static t_protocol	t_server;
	static size_t		i;

	usleep(WAIT_TIME);
	(void)context;
	(void)info;
	if (t_server.bits == 0)
		t_server.data = 0;
	if (sig == SIGUSR2 && t_server.flag == 0)
		t_server.data |= 1 << (((sizeof(int) * 8) - 1) - t_server.bits);
	else if (sig == SIGUSR2 && t_server.flag == 1)
		t_server.data |= 1 << (((sizeof(char) * 8) - 1) - t_server.bits);
	t_server.bits++;
	msg_len_received(&t_server);
	msg_received(&t_server, &i, info->si_pid);
	send_bit(info->si_pid, 0, 0);
}

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
