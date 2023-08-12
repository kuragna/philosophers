/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 20:46:16 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/12 16:07:01 by aabourri         ###   ########.fr       */
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

#define PRINT(str) printf("%ld %d %s\n", current_time, id, str)

size_t	philo_time_input(int *time, int flag)
{
	return (time[flag]);
}

void	philo_pick_fork(int id, int idx, int *hand, t_data *data)
{
	const size_t current_time = philo_time(data->started_time);

	(void)current_time;

	if (*hand)
		return ;
	(void)id;
	pthread_mutex_lock(data->forks + idx);
	*hand = 1;
// 	printf("%ld %d has taken a fork\n", philo_time(data->started_time), id);
	PRINT("has taken a fork");
}

typedef void (*t_callback)(int, int, int*, t_data*);

void	philo_eat(t_philo *philo, t_callback callback)
{
	const int right = philo->id % philo->data->number_of_philos;
	const int left = philo->id - 1;
	size_t	current_time;
	int		id;

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
	id = philo->id;
	current_time = philo_time(philo->data->started_time);
// 	printf("%ld %d is eating\n", philo_time(philo->data->started_time), philo->id);
	PRINT("is eating");
	usleep(philo->data->time_to_eat * 1000);

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
	size_t current_time;
	int	id;

	if (!philo->has_ate)
		return ;

	id = philo->id;
	current_time = philo_time(philo->data->started_time);
	PRINT("is sleeping");
// 	printf("%ld %d is sleeping\n", philo_time(started_time), philo->id);
	usleep(philo->data->time_to_sleep * 1000);
	id = philo->id;
	current_time = philo_time(philo->data->started_time);
// 	printf("%ld %d is thinking\n", philo_time(started_time), philo->id);
	usleep(philo->data->time_to_think * 1000);
	philo->has_ate = 0;
}

void	*philo_routine(void *arg)
{
	t_philo	*philo = arg;

	while (!philo->data->should_stop)
	{
		philo_eat(philo, philo_pick_fork);
		philo_sleep_think(philo);
	}
	return (NULL);
}

int	philo_init(char **args)
{
	size_t	i;
	t_data	data;
	t_philo *philos;

	philo_get_data(&data, args);

	philos = malloc(sizeof(*philos) * data.number_of_philos);
	if (philos == NULL)
		return (0);
	i = 0;
	while (i < data.number_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].data = &data;
		philos[i].right_hand = 0;
		philos[i].left_hand = 0;
		philos[i].has_ate = 0;

		i += 1;
	}

	i = 0;
	while (i < data.number_of_philos)
	{
		pthread_create(&philos[i].thread, NULL, data.routine, philos + i); // check for errors
		i += 1;
	}

	if (philo_join(philos, data.number_of_philos) == -1)
		return (0);
	return (1);
}

int main(int argc, char **argv)
{
	const int err = philo_parse_input(argv + 1);
	philo_out = fopen("philo_out.txt", "w");

	if (err)
	{
		if (--argc == 4 || argc == 5)
			philo_init(argv + 1); // without optional
		else
			printf("Error: Invalid arguments\n");
	}
	else
		printf("Error: Invalid arguments\n");
	return (0);
}