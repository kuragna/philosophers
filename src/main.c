/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 20:46:16 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/14 20:48:58 by aabourri         ###   ########.fr       */
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
	*hand = 1;
	printf("%ld %d has taken a fork\n", philo_time(data->started_time), id);
}

void	philo_eat(t_philo *philo)
{
	const int right = philo->id % philo->data->number_of_philos;
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
	
	philo->last_meal = philo_time(started_time);

	philo->has_ate = 1;

	pthread_mutex_unlock(philo->data->forks + right);
	philo->right_hand = 0;

	pthread_mutex_unlock(philo->data->forks + left);
	philo->left_hand = 0;
}

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

void	*philo_routine(void *arg)
{
	t_philo	*philo = arg;

	pthread_mutex_lock(&philo->data->should_stop_mutex);
	while (!philo->data->should_stop)
	{
		pthread_mutex_unlock(&philo->data->should_stop_mutex);
		philo_eat(philo);
		philo_sleep_think(philo);
	}
	return (NULL);
}

// NOTE: We have a data race to following variables:
// 		 should_stop and last_meal_mutex.

FILE	*out;

void	*philo_death(t_philo *philo)
{
	size_t	i;
	t_data	*data;

	i = 0;
	data = philo->data;
	while (!philo[i].data->should_stop)//!philo->data->should_stop)
	{
		while (i < data->number_of_philos)
		{
			pthread_mutex_lock(&data->last_meal_mutex);
			if (philo_time(data->started_time) - philo[i].last_meal >= data->time_to_die)
			{
				printf("%ld %d is died\n", philo_time(data->started_time), philo[i].id);
				pthread_mutex_lock(&data->should_stop_mutex);
				philo[i].data->should_stop = 1;
				pthread_mutex_unlock(&data->should_stop_mutex);
// 				pthread_detach(philo[i].thread);
// 				return (NULL);
				break;
			}
			pthread_mutex_unlock(&data->last_meal_mutex);
			i += 1;
		}
		i = 0;
	}
	return NULL;
}

int	philo_init(t_data *data)
{
	size_t	i;
	t_philo *philo;

	philo = malloc(sizeof(*philo) * data->number_of_philos);
	if (philo == NULL)
		return (0);

	i = 0;

	pthread_mutex_init(&data->last_meal_mutex, NULL);
	pthread_mutex_init(&data->should_stop_mutex, NULL);

	while (i < data->number_of_philos)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].right_hand = 0;
		philo[i].left_hand = 0;
		philo[i].has_ate = 0;

		// TODO: check if returns nonzero.
		pthread_create(&philo[i].thread, NULL, data->routine, philo + i);
		i += 1;
	}
	philo_death(philo);
	// TODO: check if returns nonzero.
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