/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 20:13:24 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/05 18:11:19 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	philo_check_input(char **args, const int args_size)
{
	char	*arg;
	int		i;

	if (args_size < 4 || args_size > 5)
	{
		philo_error("Usage: ./philo number_of_philos time_to_die time_to_eat");
		return (philo_error(" time_to_sleep [number_of_meals]\n"));
	}
	i = 0;
	while (args[i] != NULL)
	{
		arg = args[i];
		while (*arg)
		{
			if (!(48 <= *arg && 57 >= *arg))
			{
				return (philo_error("Error: Invalid arguments\n"));
			}
			arg++;
		}
		i += 1;
	}
	return (true);
}

bool	philo_get_data(t_data *data, char **args)
{
	data->philo_number = ft_atoi(args[PHILO_NUMBER]);
	data->time_to_die = ft_atoi(args[T_DIE]);
	data->time_to_eat = ft_atoi(args[T_EAT]);
	data->time_to_sleep = ft_atoi(args[T_SLEEP]);
	if (!data->philo_number || !data->time_to_die || !data->time_to_sleep
		|| !data->time_to_eat)
		return (philo_error("Error: Invalid arguments\n"));
	data->meal_number = ft_atoi(args[MEAL_NUMBER]);
	data->meal_flag = 0;
	data->log = true;
	data->should_stop = false;
	data->forks = malloc(sizeof(*data->forks) * data->philo_number);
	philo_push_mem(data->mem, data->forks);
	if (!philo_mutex_init(data))
		return (philo_error("Error: Could not initial mutex\n"));
	return (true);
}

void	philo_usleep(long long time)
{
	const long long	start = philo_get_time() + time;

	while (philo_get_time() < start)
		usleep(100);
}

long long	philo_get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

bool	philo_error(const char *str)
{
	while (*str != '\0')
	{
		write(2, str++, 1);
	}
	return (false);
}
