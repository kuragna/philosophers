/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:40:05 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/22 20:50:07 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static bool	philo_parse_input(char **args, const int size)
{
	char	*str;
	int		i;

	i = 0;
	if (size < 4 || size > 5)
	{
		philo_error(NULL, "Usage: ./philo time_to_die time_to_eat");
		return (philo_error(NULL, " time_to_sleep [number_of_meals]\n"));
	}
	while (args[i] != NULL)
	{
		str = args[i];
		while (*str)
		{
			if (!(48 <= *str && 57 >= *str))
				return (philo_error(NULL, "Error: Invalid arguments\n"));
			str++;
		}
		i += 1;
	}
	return (true);
}

int	philo_get_data(t_data *data, char **args, const int size)
{
	if (!philo_parse_input(args, size))
		return (0);
	data->number_philos = PHILO_ARG(NUMBER_OF_PHILOS); //GET_ARG(NUMBER_OF_PHILOS);
	data->time_to_die = PHILO_ARG(T_DIE); //GET_ARG(T_DIE);
	data->time_to_eat = PHILO_ARG(T_EAT); //GET_ARG(T_EAT);
	data->time_to_sleep = PHILO_ARG(T_SLEEP);//GET_ARG(T_SLEEP);

	if (!data->number_philos || !data->time_to_die || !data->time_to_sleep
		|| !data->time_to_eat)
		return (philo_error(NULL, "Error: Invalid arguments\n"));
	data->time_to_think = (data->time_to_eat * 2) - data->time_to_sleep;
	data->meal_number = PHILO_ARG(MEAL_NUMBER);//GET_ARG(NOTEPME);
	data->started_time = philo_time(0);
	data->should_stop = false;
	data->death_mutex = philo_mutex_init(data->number_philos);
	if (!data->death_mutex)
		return (philo_error(NULL, "Error: Could not initial mutex\n"));
	data->forks = philo_mutex_init(data->number_philos);
	if (!data->forks)
		return (philo_error(data->death_mutex,
				"Error: Could not initial mutex\n"));
	return (true);
}
