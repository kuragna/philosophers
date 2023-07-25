/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:14:08 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/25 19:42:32 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <errno.h>
#include <limits.h>

#define MUTEX_INIT(mutex) pthread_mutex_init(&mutex, NULL)
#define LOCK(mutex) pthread_mutex_lock(&mutex)
#define UNLOCK(mutex) pthread_mutex_unlock(&mutex)
#define JOIN(thread) pthread_join(thread, NULL)
#define FORKS_STATUS() philo_forks_status(philo)

#define ON_TABLE 1
#define ON_HAND 0

// TODO; check every argument if has value <= 0
// TODO: make condition for 1 philospher for 1 fork
// TODO: change ID's
// TODO: fix pick up left fork

enum	e_philo_time
{
	T_DIE = 0,
	T_EAT,
	T_SLEEP,
	T_THINK,
};

typedef struct s_philo
{	
	int			*times;
	int			*forks;
	int			id;
	int			right_hand;
	int			left_hand;
	int			number_of_philos;
	pthread_mutex_t	*mutex_ptr;
	pthread_mutex_t	*hand_mutex_ptr;
	pthread_t	thread;
}	t_philo;

typedef struct s_data
{
	int	times[4];
	int	*forks;
	pthread_mutex_t	mutex;
	pthread_mutex_t	hand_mutex;
	t_philo *philos;
}	t_data;

void	find_leaks()
{
	system("leaks -q philo");
}

void	philo_forks_status(t_philo *philo)
{
	pthread_mutex_lock(philo->mutex_ptr);
	printf("------------ forks status ------------\n");
	printf("{ ");
	for (int i = 0; i < philo->number_of_philos; i++)
	{
		if (!philo->forks[i])
			printf("HAND, ");
		else
			printf("TABLE, ");
	}
	printf("}\n");
	printf("------------ forks status ------------\n");
	pthread_mutex_unlock(philo->mutex_ptr);
}

time_t	philo_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec);
}

int	*philo_memset(int *ptr, int n, size_t	size)
{
	size_t	i;

	i = 0;
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



void	philo_think(t_philo *philo)
{
	printf("%zu %d is thinking\n", philo_time(), philo->id + 1);
	usleep(philo->times[T_THINK]);
}

void	philo_sleep(t_philo *philo)
{
	printf("%zu %d is sleeping\n", philo_time(), philo->id + 1);
	usleep(philo->times[T_SLEEP]);
}

void	philo_eat(t_philo *philo)
{
	if (!philo->right_hand || !philo->left_hand)
		return ;
	printf("%zu %d is eating\n", philo_time(), philo->id + 1);
	usleep(philo->times[T_EAT]);
}

// TODO: philosephers can't pick up two fork at the same time.
// NOTE: he can't pick up a fork if already with another philosopher.
// NOTE: try to pick up a fork if there are two or by using odd and even



// NOTE: 1 means fork on the table
// NOTE: 0 means philosopher has taken it




void	philo_pick_left_fork(t_philo *philo)
{
	const time_t time = philo_time();
	const int index = philo->id - 1;

	if (philo->number_of_philos == 1 || philo->left_hand)
		return ;

	pthread_mutex_lock(philo->mutex_ptr);
	philo->left_hand = philo->forks[index];
	philo->forks[index] = ON_HAND;
	printf("%zu %d has taken a left fork\n", time, philo->id);
	pthread_mutex_unlock(philo->mutex_ptr);
}

void	philo_pick_right_fork(t_philo *philo)
{
	const time_t time = philo_time();	
	const int index = philo->id % philo->number_of_philos;

	if (philo->right_hand)
		return ;

	pthread_mutex_lock(philo->mutex_ptr);
	philo->right_hand = philo->forks[index];
	philo->forks[index] = ON_HAND;
	printf("%zu %d has taken a right fork\n", time, philo->id);
	pthread_mutex_unlock(philo->mutex_ptr);
}

void	*philo_routine(t_philo *philo)
{
	while (1)
	{
		// TODO: think
		FORKS_STATUS();
		philo_pick_right_fork(philo);
// 		philo_pick_left_fork(philo);
		sleep(1);
	}
	return NULL;
}

#define GET_ARG(time) ft_atoi(args[time])


t_data	*philo_init(char **args, const int number_of_philos)
{
	int	i;
	t_data	*data;

	i = -1;
	data = malloc(sizeof(*data));

	data->times[T_DIE] = GET_ARG(T_DIE);
	data->times[T_EAT] = GET_ARG(T_EAT);
	data->times[T_SLEEP] = GET_ARG(T_SLEEP);
	data->times[T_THINK] = 4242;

	data->philos = malloc(sizeof(*data->philos) * number_of_philos);
	data->forks = malloc(sizeof(*data->forks) * number_of_philos);

	philo_memset(data->forks, 1, number_of_philos);

	pthread_mutex_init(&data->mutex, NULL);
	pthread_mutex_init(&data->hand_mutex, NULL);

	while (++i < number_of_philos)
	{
		data->philos[i].id = i + 1;

		data->philos[i].times = data->times;
		data->philos[i].mutex_ptr = &data->mutex;

		data->philos[i].hand_mutex_ptr = &data->hand_mutex;

		data->philos[i].number_of_philos = number_of_philos;
		data->philos[i].forks = data->forks;

		pthread_create(&data->philos[i].thread, NULL, (void*)philo_routine, data->philos + i);
	}
	return (data);
}

int	philo_join(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos->number_of_philos)
	{
		if (JOIN(data->philos[i].thread) == -1)
			return (0);
	}
	return (1);
}
int main(int argc, char **argv)
{
	if (argc != 5 || !philo_parse_input(argv + 1))
	{
		printf("Error: %s\n", strerror(EINVAL));
		return (1);
	}
	int	number_of_philos = ft_atoi(argv[1]);
	t_data *data = philo_init(argv + 2, number_of_philos);
	philo_join(data);
	return (0);
}