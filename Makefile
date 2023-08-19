NAME		= philo
CC			= gcc
CFLAGS		= -Wall -Werror -Wextra -g3 #-fsanitize=thread
RM			= rm -fr
SRC			= main.c \
			  ft_atoi.c \
			  philo_error.c \
			  philo_mutex_init.c \
			  philo_join.c \
			  philo_time.c \
			  philo_get_data.c \
			  philo_reset_mem.c \
			  philo_routine_each_time.c \
			  find_leaks.c 
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
