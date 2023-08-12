/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 20:46:16 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/12 18:56:25 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// NOTE: philosephers can't pick up two fork at the same time.
// NOTE: they can't pick up a fork if already with another philosopher.
// NOTE: try to pick up a fork if there are two or by using odd and even
// NOTE: number of philosphers can eat at same time N/2
// NOTE: make sure program works if optional argument is 0
// TODO: check usleep for errors.
// TODO: don't allocate memory if number_of_philos is 0
// NOTE: how can one philospher die if he trying pocked up left fork


t_mutex *test;
FILE	*philo_out;

#define PICK_RIGHT() philo_pick_fork(philo->id, RIGHT, &philo->right_hand, philo->data)
#define PICK_LEFT() philo_pick_fork(philo->id, LEFT, &philo->left_hand, philo->data)

//#define PRINT(str) printf("%ld %d %s\n", current_time, id, str)


void	philo_pick_fork(int id, int idx, int *hand, t_data *data)
{
	const size_t current_time = philo_time(data->started_time);

	(void)current_time;
	(void)id;

	if (*hand)
		return ;
	pthread_mutex_lock(data->forks + idx);
	*hand = 1;
	printf("%ld %d has taken a fork\n", philo_time(data->started_time), id);
}


void	philo_eat(t_philo *philo, t_callback callback)
{
	const int right = philo->id % philo->data->number_of_philos;
	const int left = philo->id - 1;
	const size_t started_time = philo->data->started_time;

	if (philo->id % 2 == 0)
	{
		callback(philo->id, left, &philo->left_hand, philo->data);
		callback(philo->id, right, &philo->right_hand, philo->data);
	}
	else
	{
		callback(philo->id, right, &philo->right_hand, philo->data);
		callback(philo->id, left, &philo->left_hand, philo->data);
	}

	if (!philo->right_hand || !philo->left_hand)
		return ;

	printf("%ld %d is eating\n", philo_time(started_time), philo->id);
	usleep(philo->data->time_to_eat * M_SEC);
	
	philo->last_meal = philo_time(started_time);

	philo->has_ate = 1;

	pthread_mutex_unlock(philo->data->forks + right);
	philo->right_hand = 0;

	pthread_mutex_unlock(philo->data->forks + left);
	philo->left_hand = 0;
}

// time = curr_time - started_time
// time - time_to_eat > time_to_die ---> dies


void	philo_sleep_think(t_philo *philo)
{
	const size_t started_time = philo->data->started_time;

	(void)started_time;
	if (!philo->has_ate)
		return ;

	printf("%ld %d is sleeping\n", philo_time(started_time), philo->id);
	usleep(philo->data->time_to_sleep * M_SEC);
	printf("%ld %d is thinking\n", philo_time(started_time), philo->id);
	usleep(philo->data->time_to_think * M_SEC);
	philo->has_ate = 0;
}

void	print_test(t_philo *philo)
{
	printf("-----------------------------------\n");
	printf("last meal  : %ld\n", philo->last_meal);
	printf("time_to_die: %ld\n", philo->data->time_to_die);
	printf("current    : %ld\n", philo_time(philo->data->started_time));
	printf("%ld\n", philo_time(philo->data->started_time) - philo->last_meal);
	printf("-----------------------------------\n");
}

void	check_death(t_philo *philo)
{
	const size_t started_time = philo->data->started_time;
	if (philo_time(started_time) - philo->last_meal >= philo->data->time_to_die)
	{
		printf("here\n");
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo = arg;

	while (!philo->data->should_stop)
	{
		philo_eat(philo, philo_pick_fork);
		philo_sleep_think(philo);
		check_death(philo);
	}
	return (NULL);
}


int	philo_init(t_data *data)
{
	size_t	i;
	t_philo *philos;

	philos = malloc(sizeof(*philos) * data->number_of_philos);
	if (philos == NULL)
		return (0);
	i = 0;
	while (i < data->number_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].right_hand = 0;
		philos[i].left_hand = 0;
		philos[i].has_ate = 0;

		pthread_create(&philos[i].thread, NULL, data->routine, philos + i); // check for errors
		i += 1;
	}
	if (philo_join(philos, data->number_of_philos) == -1)
		return (0);
	return (1);
}


int main(int argc, char **argv)
{
	atexit(find_leaks);
	t_data data;
	const int err = philo_get_data(&data, argv + 1);
	philo_out = fopen("philo_output.txt", "w");

	if (err)
	{
		if (--argc == 4 || argc == 5)
			philo_init(&data); // without optional
		else
			printf("Error: Invalid arguments\n");
	}
	else
		printf("Error: Invalid arguments\n");
	return (0);
}