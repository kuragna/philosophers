
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:14:08 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/28 18:14:11 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <errno.h>
#include <limits.h>

typedef unsigned int u_int_t;

#define RED 	"\e[0;31m"
#define GREEN 	"\e[0;32m"
#define RESET 	"\e[0m"

#define GET_ARG(time) ft_atoi(args[time])
#define JOIN(thread) pthread_join(thread, NULL)
#define FORKS_STATUS() philo_forks_status(philo)
#define PRINT_HANDS() printf("ID: %d | hands[%d, %d]\n", \
		philo->id,\
		philo->right_hand, \
		philo->left_hand)
#define PRINT() printf("ID: %d\n", philo->id)


#define P_THINKING 1
#define P_EATING 2
#define P_SLEEPING 3

#define NUMBER_ARGS (3)

enum	e_philo_input
{
	NUMBER_OF_PHILOS = 0,
	T_DIE,
	T_EAT,
	T_SLEEP,
	T_THINK,
};

typedef struct s_data
{
	int times[4];
	int	number_of_philos;
	int	*forks;
	pthread_mutex_t	mutex;
	pthread_mutex_t *mutexes; // try it later
}	t_data;

typedef struct s_philo
{	
	int			id; // 4
	int 		right_hand; // 4
	int 		left_hand; // 4
	int			status; // 4
	t_data		*data;
	pthread_t	thread; // 8
}	t_philo; // 52 != 56

int		philo_get_fork(int *forks, int index, pthread_mutex_t *mutex);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_die(t_philo *philo);
void	philo_think(t_philo *philo);

void	philo_put_left_fork(t_philo *philo);
void	philo_put_right_fork(t_philo *philo);

void	philo_pick_left_fork(t_philo *philo);
void	philo_pick_right_fork(t_philo *philo);


void	find_leaks()
{
	system("leaks -q philo");
}

void	philo_forks_status(t_philo *philo)
{
// 	pthread_mutex_lock(&philo->data->mutex);
// 	printf("------------ forks status ------------\n");
	printf("ID: %d = { ", philo->id);
	for (int i = 0; i < philo->data->number_of_philos; i++)
	{
		if (!philo->data->forks[i])
			printf("HAND");
		else
			printf("TABLE");
		if (i != philo->data->number_of_philos - 1)
			printf(", ");
	}
	printf(" }\n");
// 	printf("------------ forks status ------------\n");
// 	pthread_mutex_unlock(&philo->data->mutex);
}


time_t	philo_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec);
}

int	*philo_fill(int n, size_t size)
{
	size_t	i;
	int	*ptr;

	i = 0;
	ptr = malloc(sizeof(*ptr) * size);
	if (ptr == NULL)
		return (NULL);
	while (i < size)
	{
		*(ptr + i) = n;
		i++;
	}
	return (ptr);
}

int	philo_parse_input(char **args)
{
	char	*str;
	int		i;

	i = -1;
	while (args[++i] != NULL)
	{
		str = args[i];
		while (*str)
		{
			if (!ft_isdigit(*str))
				return (FALSE);
			str++;
		}
	}
	return (TRUE);
}




#if 0

void	philo_sleep(t_philo *philo)
{
	printf("%zu %d is sleeping\n", philo_time(), philo->id);
	usleep(philo->times[T_SLEEP]);
}


// TODO: philosephers can't pick up two fork at the same time.
// NOTE: he can't pick up a fork if already with another philosopher.
// NOTE: try to pick up a fork if there are two or by using odd and even



// NOTE: 1 means fork on the table
// NOTE: 0 means philosopher has taken it

// NOTE: number of philosphers can eat at same time N/2

void	philo_pick_right_fork(t_philo *philo)
{
// 	const time_t time = philo_time();	
	const int index = philo->id % philo->number_of_philos;
	const int fork = philo_get_fork(philo->forks, index, philo->mutex_ptr);

	if (philo->right_hand || !fork)
		return ;
	pthread_mutex_lock(philo->mutex_ptr);
	philo->right_hand = philo->forks[index];
	philo->forks[index] = 0;
// 	printf("%zu %d has taken a right fork\n", time, philo->id);
	pthread_mutex_unlock(philo->mutex_ptr);
}

void	philo_pick_left_fork(t_philo *philo)
{
// 	const time_t time = philo_time();
	const int index = philo->id - 1;
	const int fork = philo_get_fork(philo->forks, index, philo->mutex_ptr);

	if (philo->number_of_philos == 1)
		return ;
	if (philo->id % 2 != 1 && philo->right_hand != 1)
		return ;
	if (philo->left_hand || !fork)
		return ;
	pthread_mutex_lock(philo->mutex_ptr);
	philo->left_hand = philo->forks[index];
	philo->forks[index] = 0;
// 	printf("%zu %d has taken a left fork\n", time, philo->id);
	pthread_mutex_unlock(philo->mutex_ptr);
}
#endif

// int	philo_check_fork(int *forks, int index, pthread_mutex_t *mutex)
int	philo_check_fork(t_data *data, int index)
{
	int	fork;


	pthread_mutex_lock(data->mutexes + index);
	fork = data->forks[index];
	pthread_mutex_unlock(data->mutexes + index);

	// one mutex
	pthread_mutex_lock(&data->mutex);
	fork = data->forks[index];
	pthread_mutex_unlock(&data->mutex);
	return (fork);
}


void	philo_eat(t_philo *philo)
{
	if (!philo->right_hand || !philo->left_hand)
		return ;
// 	printf("%zu %d is eating\n", philo_time(), philo->id);
	usleep(philo->data->times[T_EAT]);
	philo_put_right_fork(philo);
	philo_put_left_fork(philo);
}


void	philo_put_left_fork(t_philo *philo)
{
	const int index = philo->id - 1;
	

	if (!philo->left_hand)
		return ;

	pthread_mutex_lock(&philo->data->mutex);
	philo->data->forks[index] = philo->left_hand;
	philo->left_hand = 0;
	pthread_mutex_unlock(&philo->data->mutex);
}

void	philo_put_right_fork(t_philo *philo)
{
	const int index = philo->id % philo->data->number_of_philos;

	if (!philo->right_hand)
		return ;

	pthread_mutex_lock(&philo->data->mutex);
	philo->data->forks[index] = philo->right_hand;
	philo->right_hand = 0;
	pthread_mutex_unlock(&philo->data->mutex);
}

void	philo_think(t_philo *philo)
{
	if (philo->right_hand && philo->left_hand)
		return ;
// 	printf("%zu %d is thinking\n", philo_time(), philo->id);
	usleep(philo->data->times[T_THINK - 1]);
}

void	philo_pick_left_fork(t_philo *philo)
{
	const int index = philo->id - 1;
	const int fork_state = philo_check_fork(philo->data, index);

	if (philo->data->number_of_philos == 1)
		return ;
	if (philo->left_hand || !fork_state)
		return ;
	pthread_mutex_lock(&philo->data->mutex);
	philo->left_hand = philo->data->forks[index];
	philo->data->forks[index] = 0;
// 	printf("%zu %d has taken a left fork\n", philo_time(), philo->id);
	pthread_mutex_unlock(&philo->data->mutex);
}

void	philo_pick_right_fork(t_philo *philo)
{
	const int index = philo->id % philo->data->number_of_philos;
	const int fork_state = philo_check_fork(philo->data, index);
	const int left_fork = philo_check_fork(philo->data, philo->id - 1);

	if (philo->id % 2 != 1 && !left_fork)
		return ;
	if (philo->right_hand || !fork_state)
		return ;
	pthread_mutex_lock(&philo->data->mutex);
	philo->right_hand = philo->data->forks[index];
	philo->data->forks[index] = 0;
// 	printf("%zu %d has taken a right fork\n", philo_time(), philo->id);
	pthread_mutex_unlock(&philo->data->mutex);
}

void	*philo_routine(t_philo *philo)
{
	while (1)
	{
		philo_think(philo);
		philo_pick_right_fork(philo);
		philo_pick_left_fork(philo);
		PRINT_HANDS();
		philo_eat(philo);
		sleep(1);
	}
	return NULL;
}

// ---------------------------- main --------------------------------------


int	philo_join(t_philo *philo)
{
	const u_int_t number_of_philos = philo[0].data->number_of_philos;
	u_int_t i;

	i = 0;
	while (i < number_of_philos)
	{
		pthread_join(philo[i].thread, NULL);
	}
	return (1);
}

int	philo_init(char **args)
{
	int	i;
	t_data	*data;
	t_philo *philos;

	i = 0;
	data = malloc(sizeof(*data)); // check for null
	data->number_of_philos = GET_ARG(NUMBER_OF_PHILOS);
	while (i < NUMBER_ARGS)
	{
		data->times[i] = GET_ARG(i + 1);
		i++;
	}
	i = 0;
	data->forks = philo_fill(1, data->number_of_philos); // check for null

	data->mutexes = malloc(sizeof(*data->mutexes) * data->number_of_philos);

	for (int i = 0; i < data->number_of_philos; i++)
	{
		pthread_mutex_init(data->mutexes + i, NULL);
	}

	pthread_mutex_init(&data->mutex, NULL);

	philos = malloc(sizeof(*philos) * data->number_of_philos); // check for null
	// TODO: make sure init all variables need.
	while (i < data->number_of_philos)
	{
		philos[i].id = i + 1; // set philosopher's id
		philos[i].data = data; // shared data with all philosophers
		philos[i].right_hand = 0;
		philos[i].left_hand = 0;

		pthread_create(&philos[i].thread, NULL, (void*)philo_routine, philos + i); // check for error
		i++;
	}
	return (philo_join(philos));
}

int main(int argc, char **argv)
{
	if (argc != 5 || !philo_parse_input(argv + 1))
	{
		printf("Error: %s\n", strerror(EINVAL));
		return (1);
	}
	philo_init(argv + 1);
	return (0);
}