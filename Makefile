NAME		= philo
CC			= gcc
CFLAGS		= -Wall -Werror -Wextra -g3 -fsanitize=thread
RM			= rm -fr
SRC			= main.c \
			  ft_atoi.c \
			  ft_isdigit.c 
SRC			:= $(addprefix src/, $(SRC))
OBJS		= $(SRC:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

$(NAME): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $^
clean:
	$(RM) $(OBJS)
fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
