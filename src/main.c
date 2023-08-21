/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 20:46:16 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/21 17:23:34 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	philo_pick_fork(int id, int pos, int *hand, t_data *data)
{
	if (*hand)
		return ;
	pthread_mutex_lock(data->forks + pos);
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
		if (philo->data->number_philos == 1)
		{
			pthread_mutex_unlock(philo->data->forks);
			return ;
		}
		philo_pick_fork(philo->id, left, &philo->left_hand, philo->data);
	}

	if (!philo->right_hand || !philo->left_hand)
		return ;


	printf("%ld %d is eating\n", philo_time(started_time), philo->id);
	usleep(philo->data->time_to_eat * M_SEC);

	pthread_mutex_lock(philo->data->death_mutex + left);
	philo->last_meal = philo_time(started_time);
	philo->eat_count += 1;
	pthread_mutex_unlock(philo->data->death_mutex + left);

	philo->has_ate = TRUE;
	pthread_mutex_unlock(philo->data->forks + right);
	philo->right_hand = FALSE;

	pthread_mutex_unlock(philo->data->forks + left);
	philo->left_hand = FALSE;
}

void	philo_sleep_think(t_philo *philo)
{
	const size_t started_time = philo->data->started_time;

// 	pthread_mutex_lock(&philo->data->should_stop_mutex);
// 	philo->should_stop = philo->data->should_stop;
// 	pthread_mutex_unlock(&philo->data->should_stop_mutex);

	if (!philo->has_ate)
		return ;

	printf("%ld %d is sleeping\n", philo_time(started_time), philo->id);
	usleep(philo->data->time_to_sleep * M_SEC);
	printf("%ld %d is thinking\n", philo_time(started_time), philo->id);
	usleep(philo->data->time_to_think * M_SEC);
	philo->has_ate = FALSE;
}

// TODO: make philosophers eat each time, without separate function.


FILE	*out;

//notepme;


void	*philo_routine(void *arg)
{
	t_philo *philo;
	int	should_stop;

	philo = arg;
	
	while (1)
	{
		pthread_mutex_lock(philo->data->death_mutex + (philo->id - 1));
		should_stop = philo->data->should_stop;
		pthread_mutex_unlock(philo->data->death_mutex + (philo->id - 1));

		if (should_stop)
			return NULL;
		philo_eat(philo);
		philo_sleep_think(philo);
	}
	return NULL;
}

void	philo_death(t_philo *philo)
{
	size_t	i;
	int		stop;
	size_t	curr_time;
	size_t	last_meal;

	stop = FALSE;
	while (!stop)
	{
		i = 0;
		while (i < philo->data->number_philos)
		{
			pthread_mutex_lock(philo->data->death_mutex + (philo[i].id - 1));
			if (philo[i].eat_count == philo->data->notepme && philo->data->notepme != 0)
			{
				philo[i].data->should_stop = TRUE;
				stop = TRUE;
			}
			curr_time = philo_time(philo->data->started_time);
			last_meal = philo[i].last_meal;
			if (curr_time - last_meal >= philo->data->time_to_die)
			{
				philo[i].data->should_stop = TRUE;
				printf("%ld %d is died\n", curr_time, philo[i].id);
				pthread_mutex_unlock(philo->data->death_mutex + (philo[i].id - 1));
				stop = TRUE;
			}
			pthread_mutex_unlock(philo->data->death_mutex + (philo[i].id - 1));
			i += 1;
		}
	}
}

int	philo_init(t_data *data)
{
	size_t	i;
	int	err;
	t_philo *philo;

	philo = malloc(sizeof(*philo) * data->number_philos); // TODO: free memory.
	if (!philo)
		return (FALSE);

	i = 0;
	data->evil = FALSE;
	pthread_mutex_init(&data->should_stop_mutex, NULL);
	while (i < data->number_philos)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].has_ate = FALSE;
		philo[i].left_hand = FALSE;
		philo[i].right_hand = FALSE;
		philo[i].should_stop = FALSE;
		philo[i].eat_count = 0;
		i += 1;
	}
	i = 0;
	while (i < data->number_philos)
	{
		err = pthread_create(&philo[i].thread, NULL, philo_routine, philo + i);
		if (err != 0)
		{
			philo_reset_mem(philo);
			return (philo_error(NULL, "Error: Could not create thread"));
		}
		i += 1;
	}
	philo_death(philo);
	philo_join(philo);
	return (philo_reset_mem(philo));
}


int main(int argc, char **argv)
{
	out = fopen("out.txt", "w");
	t_data data;

	if (!philo_get_data(&data, argv + 1, argc - 1))
		return (1);
	if (!philo_init(&data))
		return (1);
	return (0);
}