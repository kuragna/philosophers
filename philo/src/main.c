/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 20:46:16 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/04 18:37:34 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// TODO: Date race
static void	philo_pick_fork(t_philo *philo, const int idx)
{
	pthread_mutex_lock(philo->data->forks + idx);
	philo_log(philo, "has taken a fork");
}

// TODO: 2 Date race
static void	philo_routine(t_philo *philo)
{
	const int	right = philo->id % philo->data->philo_number;
	const int	left = philo->id - 1;

	philo_pick_fork(philo, right);
	philo_pick_fork(philo, left);
	philo_log(philo, "is eating");
	philo_usleep(philo->data->time_to_eat);
	pthread_mutex_lock(&philo->data->eat_mutex);
	philo->last_meal = philo_get_time() - philo->data->started_time;
	philo->eat_count += 1;
	pthread_mutex_unlock(&philo->data->eat_mutex);
	pthread_mutex_unlock(philo->data->forks + right);
	pthread_mutex_unlock(philo->data->forks + left);
	philo_log(philo, "is sleeping");
	philo_usleep(philo->data->time_to_sleep);
	philo_log(philo, "is thinking");
}

// TODO: 2 data race
void	*routine(void *arg)
{
	t_philo	*philo;
	bool	should_stop;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		philo_usleep(50);
	while (true)
	{
		pthread_mutex_lock(&philo->data->stop_mutex);
		should_stop = philo->should_stop;
		pthread_mutex_unlock(&philo->data->stop_mutex);
		if (should_stop)
		{
			pthread_mutex_lock(&philo->data->meal_mutex);
			philo->data->meal_flag += 1;
			pthread_mutex_unlock(&philo->data->meal_mutex);
			return (NULL);
		}
		philo_routine(philo);
	}
	return (NULL);
}

static bool	philo_init(t_data *data)
{
	int		err;
	size_t	i;
	t_philo	*philo;

	philo = malloc(sizeof(*philo) * data->philo_number);
	if (!philo)
		return (philo_error("Error: Failed to allocate memory\n"));
	philo_push_mem(data->mem, philo);
	memset(philo, 0, sizeof(*philo) * data->philo_number);
	data->started_time = philo_get_time();
	i = -1;
	while (++i < data->philo_number)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		err = pthread_create(&philo[i].thread, NULL, routine, philo + i);
		if (err != 0)
			return (philo_error("Error: Could not create a thread\n"));
	}
	philo_death(philo);
	return (true);
}

// TODO: destroy mutex
int	main(int argc, char **argv)
{
	t_data	data;
	t_mem	mem;
	int		exit_status;

	atexit(find_leaks);
	exit_status = 0;
	if (!philo_check_input(argv + 1, argc - 1))
		return (1);
	if (!philo_init_mem(&mem))
	{
		philo_error("Error: Failed to allocate memory\n");
		return (1);
	}
	data.mem = &mem;
	if (!philo_get_data(&data, argv + 1))
		exit_status = 1;
	else
	{
		if (!philo_init(&data))
			exit_status = 1;
	}
	philo_reset_mem(data.mem);
	return (exit_status);
}
