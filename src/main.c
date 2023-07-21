/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:14:08 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/21 18:20:42 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <errno.h>

#define MUTEX_INIT(mutex) pthread_mutex_init(&mutex, NULL)
#define LOCK(mutex) pthread_mutex_lock(&mutex)
#define UNLOCK(mutex) pthread_mutex_unlock(&mutex)
#define JOIN(thread) pthread_join(thread, NULL)

// TODO; check every argument if has value <= 0
// TODO: each philosopher should be a thread
// TODO: change errors to stderr

enum	e_philo
{
	TIME_TO_DIE = 0,
	TIME_TO_EAT,
	TIME_TO_SLEEP,
	TIME_TO_THINK,
};

typedef struct s_philo
{	
	int			id;
	int			*times;
	int			*forks;
	pthread_mutex_t	*mutex_ptr;
	pthread_t	thread;
}	t_philo;

typedef struct s_data
{
	int	times[4];
	int	number_of_philos;
	int	forks;
	pthread_mutex_t	mutex;
	t_philo *philos;
}	t_data;

void	find_leaks()
{
	system("leaks -q philo");
}

time_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec);
}

int	parse_input(char **args)
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


void	philo_pick_fork(t_philo *philo)
{
	(void)philo;
	printf("Not implemented yet\n");
}

void	philo_think(int time_to_think, int id)
{
	printf("%zu %d is thinking\n", get_time(), id);
	usleep(time_to_think);
}

void	philo_sleep(int time_to_sleep, int id)
{
	printf("%zu %d is sleeping\n", get_time(), id);
	usleep(time_to_sleep);
}

void	philo_eat(int time_to_eat, int id)
{
	printf("%zu %d is eating\n", get_time(), id);
	usleep(time_to_eat);
	// NOTE: maybe we need to call philo_think after eat
}

void	*philo_routine(void *arg)
{
	t_philo *philo = (t_philo*)arg;

	while (1)
	{
		philo_pick_fork(philo);
		sleep(1);
// 		philo_eat(philo->times[TIME_TO_EAT], philo->id);
// 		philo_sleep(philo->times[TIME_TO_SLEEP], philo->id);
	}
	return (NULL);
}

#define GET_ARG(time) ft_atoi(args[time])

t_data	*philo_init(char **args, const int number_of_philos)
{
	int	i;
	t_data	*data;

	i = -1;
	data = malloc(sizeof(*data));
	data->times[TIME_TO_DIE] = GET_ARG(TIME_TO_DIE);
	data->times[TIME_TO_EAT] = GET_ARG(TIME_TO_EAT);
	data->times[TIME_TO_SLEEP] = GET_ARG(TIME_TO_SLEEP);
	data->times[TIME_TO_THINK] = 4242;
	data->philos = malloc(sizeof(*data->philos) * number_of_philos);
	data->number_of_philos = number_of_philos;
// 	data->forks = number_of_philos;
	while (++i < number_of_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].times = data->times;
		data->philos[i].forks = &data->forks;
		data->philos[i].mutex_ptr = &data->mutex;
		pthread_create(&data->philos[i].thread, NULL, philo_routine, data->philos + i);
	}
	MUTEX_INIT(data->mutex);
	return data;
}

int	philo_join(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philos)
	{
		if (JOIN(data->philos[i].thread) == -1)
			return (0);
	}
	return (1);
}
int main(int argc, char **argv)
{
	if (argc != 5 || !parse_input(argv + 1))
	{
		printf("Error: %s\n", strerror(EINVAL));
		return (1);
	}
	int	number_of_philos = ft_atoi(argv[1]);
	t_data *data = philo_init(argv + 2, number_of_philos);
	philo_join(data);
	return (0);
}
