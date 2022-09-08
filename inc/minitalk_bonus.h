/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk_bonus.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 22:56:16 by sciftci           #+#    #+#             */
/*   Updated: 2022/09/08 15:37:05 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_BONUS_H
# define MINITALK_BONUS_H

# include <signal.h>
# include <stdlib.h>
# include <unistd.h>

# define WAIT_TIME 100

int			ft_atoi(const char *str);
size_t		ft_strlen(const char *s);
void		ft_putnbr_fd(int n, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putchar_fd(char c, int fd);

void		check_sigaction_signals(struct sigaction *sa);
void		send_len(pid_t pid, int len);
void		send_char(pid_t pid, char c);
void		send_bit(pid_t pid, char bit, char flag_to_pause);

/*
  Structure to handle the protocol between server-client
  
  bits : bits received
  data : data received, can be an int (length of message) or char
  flag : 1 if length of message was received, else 0
  message : message received
  index : message index
*/
typedef struct s_protocol
{
	int		bits;
	int		data;
	int		flag;
	char	*message;
	size_t	index;
}			t_protocol;

#endif