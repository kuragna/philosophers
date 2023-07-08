NAME		= philo
CC			= gcc
CFLAGS		= -Wall -Werror -Wextra -g3 -fsanitize=address
RM			= rm -fr
SRC			= main.c \

SRC			:= $(addprefix src/, $(SRC))

all: $(NAME)

$(NAME): $(SRC)
	$(CC) -o $@ $(CFLAGS) $^
clean:
	echo "here objects"
fclean:
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
