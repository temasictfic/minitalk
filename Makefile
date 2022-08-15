# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/15 04:11:55 by sciftci           #+#    #+#              #
#    Updated: 2022/08/15 14:52:35 by sciftci          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := cc
CFLAGS := -Wall -Wextra -Werror
RM := rm -f

NAME_SERVER := server
NAME_CLIENT := client

SRC_DIR = src
SRCB_DIR = srcb
INC_DIR = inc

SRC_SERVER := src/server.c src/minitalk.c src/minitalk_utils.c
SRC_CLIENT := src/client.c src/minitalk.c src/minitalk_utils.c

SRC_SERVER_BONUS := srcb/server_bonus.c srcb/minitalk_bonus.c src/minitalk_utils.c
SRC_CLIENT_BONUS := srcb/client_bonus.c srcb/minitalk_bonus.c src/minitalk_utils.c

#=====================================================================================

all: $(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER):
	$(CC) $(CFLAGS) $(SRC_SERVER) -I ${INC_DIR} -o $(NAME_SERVER)
	
$(NAME_CLIENT):
	$(CC) $(CFLAGS) $(SRC_CLIENT) -I ${INC_DIR} -o $(NAME_CLIENT)
	
clean:

fclean: clean
	$(RM) $(NAME_SERVER) $(NAME_CLIENT)

re: fclean all

bonus: fclean
	$(CC) $(CFLAGS) $(SRC_SERVER_BONUS) -I ${INC_DIR} -o $(NAME_SERVER)
	$(CC) $(CFLAGS) $(SRC_CLIENT_BONUS) -I ${INC_DIR} -o $(NAME_CLIENT)

rebonus: fclean bonus

# .PHONY rule in order to avoid relinking
.PHONY: all clean fclean re bonus rebonus