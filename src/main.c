/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 20:46:16 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/17 17:20:26 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	philo_pick_fork(int id, int idx, int *hand, t_data *data)
{
	const size_t current_time = philo_time(data->started_time);

	(void)current_time;
	(void)id;

	if (*hand)
		return ;
	pthread_mutex_lock(data->forks + idx);
	*hand = TRUE;
	printf("%ld %d has taken a fork\n", philo_time(data->started_time), id);
}

void	philo_eat(t_philo *philo)
{
	const int right = philo->id % philo->data->number_philos;
	const int left = philo->id - 1;
	const size_t started_time = philo->data->started_time;

	if (philo->id % 2 == 0)
	{
		philo_pick_fork(philo->id, left, &philo->left_hand, philo->data);
		philo_pick_fork(philo->id, right, &philo->right_hand, philo->data);
	}
	else
	{
		philo_pick_fork(philo->id, right, &philo->right_hand, philo->data);
		philo_pick_fork(philo->id, left, &philo->left_hand, philo->data);
	}

	if (!philo->right_hand || !philo->left_hand)
		return ;

	printf("%ld %d is eating\n", philo_time(started_time), philo->id);
	usleep(philo->data->time_to_eat * M_SEC);

	pthread_mutex_lock(&philo->data->last_meal_mutex);
	philo->last_meal = philo_time(philo->data->started_time);
	pthread_mutex_unlock(&philo->data->last_meal_mutex);

	pthread_mutex_lock(&philo->data->has_ate);
	philo->has_ate = TRUE;
	pthread_mutex_lock(&philo->data->has_ate);

	pthread_mutex_unlock(philo->data->forks + right);
	philo->right_hand = FALSE;

	pthread_mutex_unlock(philo->data->forks + left);
	philo->left_hand = FALSE;
}

void	philo_sleep_think(t_philo *philo)
{
	const size_t started_time = philo->data->started_time;
	
	if (!philo->has_ate)
		return ;

	printf("%ld %d is sleeping\n", philo_time(started_time), philo->id);
	usleep(philo->data->time_to_sleep * M_SEC);
	printf("%ld %d is thinking\n", philo_time(started_time), philo->id);
	usleep(philo->data->time_to_think * M_SEC);
	philo->has_ate = FALSE;
}

void	*philo_routine(void *arg)
{
	t_philo	*philo = arg;

	pthread_mutex_lock(&philo->data->should_stop_mutex);
	int	should_stop = philo->data->should_stop;
	pthread_mutex_unlock(&philo->data->should_stop_mutex);

	while (!should_stop)
	{
// 		printf("no one died\n");
		philo_eat(philo);
		philo_sleep_think(philo);
	}

	return (NULL);
}

// NOTE: We have a data race to following variables:
// 		 should_stop and last_meal_mutex.

FILE	*out;

// TODO: try to do it recursively.

void	philo_death(t_philo *philo)
{
	size_t	i;
	size_t	last_meal;
	size_t	time;
	const size_t 	time_to_die = philo->data->time_to_die;

	i = 0;
	while (TRUE)
	{
		while (i < philo->data->number_philos)
		{
			time = philo_time(philo[i].data->started_time);
			pthread_mutex_lock(&philo[i].data->last_meal_mutex);
			pthread_mutex_lock(&philo[i].data->has_ate);
			if (philo[i].has_ate)
				last_meal = philo[i].last_meal;
			pthread_mutex_unlock(&philo[i].data->has_ate);
			pthread_mutex_unlock(&philo[i].data->last_meal_mutex);
			if (time - last_meal >= time_to_die)
			{
				pthread_mutex_lock(&philo[i].data->should_stop_mutex);
				philo[i].should_stop = TRUE;
				pthread_detach(philo[0].thread);
				pthread_mutex_unlock(&philo[i].data->should_stop_mutex);
				printf("%ld %d is died\n", time, philo[i].id);
				return ;
			}
			i += 1;
		}
		i = 0;
	}
}

int	philo_init(t_data *data)
{
	size_t	i;
	t_philo *philo;

	philo = malloc(sizeof(*philo) * data->number_philos);
	if (philo == NULL)
		return (0);

	pthread_mutex_init(&data->should_stop_mutex, NULL);
	pthread_mutex_init(&data->last_meal_mutex, NULL);
	pthread_mutex_init(&data->has_ate, NULL);

	printf("init: [1: %p | 2: %p]\n", &data->should_stop_mutex, &data->last_meal_mutex);

	i = 0;
	while (i < data->number_philos)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].right_hand = FALSE;
		philo[i].left_hand = FALSE;
		philo[i].has_ate = FALSE;
		philo[i].should_stop = FALSE;
		i += 1;
	}
	i = 0;
	while (i < data->number_philos)
	{

		// TODO: check if returns non-zero.
		pthread_create(&philo[i].thread, NULL, data->routine, philo + i);
		i += 1;
	}
	// TODO: check if returns nonzero.
	
	philo_death(philo);
// 	if (!philo_join(philo))
// 		return (0);
// 	philo_reset_mem(philo);
	return (1);
}

int main(int argc, char **argv)
{
	out = fopen("out.txt", "w");
	t_data data;
	const int err = philo_get_data(&data, argv + 1, argc - 1);

	if (err)
	{
		philo_init(&data);
	}
	else
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	return (0);
}