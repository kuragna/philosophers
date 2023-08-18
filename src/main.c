/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 20:46:16 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/18 17:47:10 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	philo_pick_fork(int id, int idx, int *hand, t_data *data)
{
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

	pthread_mutex_lock(philo->data->last_meal_mutex + left);
	philo->last_meal = philo_time(philo->data->started_time);
	pthread_mutex_unlock(philo->data->last_meal_mutex + left);

	philo->has_ate = TRUE;

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
	while (!(*philo->should_stop))
	{
		pthread_mutex_unlock(&philo->data->should_stop_mutex);
		philo_eat(philo);
		philo_sleep_think(philo);
	}
	pthread_mutex_unlock(&philo->data->should_stop_mutex);
	return (NULL);
}

FILE	*out;

// TODO: try to do it recursively.

// TODO: try should stop as pointer.
void	philo_death(t_philo *philo, const size_t size, const size_t time_to_die)
{
	size_t	i;
	size_t	time;
	size_t	last_meal;

	i = 0;
	while (1)
	{
		while (i < size)
		{
			time = philo_time(philo[i].data->started_time);

			pthread_mutex_lock(philo[i].data->last_meal_mutex + (philo[i].id - 1));
			last_meal = philo[i].last_meal;
			pthread_mutex_unlock(philo[i].data->last_meal_mutex + (philo[i].id - 1));

			if (time - last_meal >= time_to_die)
			{
				pthread_mutex_lock(&philo[i].data->should_stop_mutex);
				*philo->should_stop = TRUE;
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
	const size_t	size = data->number_philos;

	philo = malloc(sizeof(*philo) * size);
	if (philo == NULL)
		return (0);

	i = 0;
	data->last_meal_mutex = philo_mutex_init(size);
	data->forks = philo_mutex_init(size);
	data->should_stop = FALSE;
	pthread_mutex_init(&data->should_stop_mutex, NULL);
	while (i < size)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].right_hand = FALSE;
		philo[i].left_hand = FALSE;
		philo[i].has_ate = FALSE;
		philo[i].should_stop = &data->should_stop;
		i += 1;
	}
	i = 0;
	while (i < size)
	{
		// TODO: check if returns non-zero.
		pthread_create(&philo[i].thread, NULL, data->routine, philo + i);
		i += 1;
	}
	// TODO: check if returns nonzero.
	
	philo_death(philo, size, data->time_to_die);
	if (!philo_join(philo))
		return (0);
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