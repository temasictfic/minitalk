/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 22:43:30 by sciftci           #+#    #+#             */
/*   Updated: 2022/09/11 19:35:15 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <signal.h>
# include <stdlib.h>
# include <unistd.h>

# define WAIT_TIME 100

int					ft_atoi(const char *str);
size_t				ft_strlen(const char *s);
void				ft_putnbr_fd(int n, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putchar_fd(char c, int fd);

void				check_sigaction_signals(struct sigaction *sa);
void				send_char(pid_t pid, char c);
void				send_bit(pid_t pid, char bit, char flag_to_pause);

/*
  Structure to handle the protocol between server-client
  
  bits : bits received
  data : data received, can be an length of message or char
*/
typedef struct s_protocol
{
	int				bits;
	unsigned int	data;
}					t_protocol;

#endif
