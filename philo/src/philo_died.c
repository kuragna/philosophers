/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_died.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:13:51 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/04 16:53:44 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// TODO: change it
static void	test(t_mutex *mutex, bool *b, bool v)
{
	pthread_mutex_lock(mutex);
	*b = v;
	pthread_mutex_unlock(mutex);
}

static bool	philo_is_dead(t_philo *philo)
{
	const long long	curr_time = philo_get_time() - philo->data->started_time;
	long long		dead_time;
	size_t			meal_flag;
	size_t			eat_count;

	pthread_mutex_lock(&philo->data->eat_mutex);
	dead_time = curr_time - philo->last_meal;
	eat_count = philo->eat_count;
	pthread_mutex_unlock(&philo->data->eat_mutex);
	if (philo->data->meal_number && eat_count == philo->data->meal_number)
	{
		pthread_mutex_lock(&philo->data->meal_mutex);
		meal_flag = philo->data->meal_flag;
		pthread_mutex_unlock(&philo->data->meal_mutex);
		test(&philo->data->stop_mutex, &philo->should_stop, true);
		if (meal_flag == philo->data->philo_number)
			return (true);
	}
	if (dead_time >= (long long)philo->data->time_to_die)
	{
		philo_log(philo, "died");
		test(&philo->data->log_mutex, &philo->data->log, false);
		return (true);
	}
	return (false);
}

// TODO: make should_stop inside data
// NOTE: maybe doesnt work with number of meals

void	philo_death(t_philo *philo)
{
	bool	stop;
	size_t	i;
	size_t	j;

	j = -1;
	stop = false;
	while (!stop)
	{
		i = 0;
		while (i < philo->data->philo_number)
		{
			if (philo_is_dead(&philo[i]))
			{
				pthread_mutex_unlock(philo->data->forks);
				pthread_mutex_lock(&philo[i].data->stop_mutex);
				while (++j < philo->data->philo_number)
					philo[j].should_stop = true;
				pthread_mutex_unlock(&philo[i].data->stop_mutex);
				stop = true;
			}
			i += 1;
		}
	}
	philo_join(philo);
}

void	philo_log(t_philo *philo, const char *str)
{
	int			log;

	pthread_mutex_lock(&philo->data->log_mutex);
	log = philo->data->log;
	pthread_mutex_unlock(&philo->data->log_mutex);
	if (log)
	{
		printf("%lld %d %s\n",
			philo_get_time() - philo->data->started_time,
			philo->id,
			str);
	}
}
