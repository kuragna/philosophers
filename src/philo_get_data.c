/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:40:05 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/12 18:58:45 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int philo_parse_input(char **args)
{
	char	*str;
	int		i;

	i = 0;
	while (args[i] != NULL)
	{
		str = args[i];
		while (*str)
		{
			if (!(48 <= *str && 57 >= *str))
				return (0);
			str++;
		}
		i += 1;
	}
	return (1);
}

int	philo_get_data(t_data *data, char **args)
{
	if (!philo_parse_input(args))
		return (0);
	data->number_of_philos = GET_ARG(NUMBER_OF_PHILOS);
	data->time_to_eat = GET_ARG(T_EAT);
	data->time_to_sleep = GET_ARG(T_SLEEP);
	data->time_to_die = GET_ARG(T_DIE);

	if (!data->number_of_philos || !data->time_to_die || !data->time_to_sleep
		|| !data->time_to_eat)
		return (0);

	data->time_to_think = (data->time_to_eat * 2) - data->time_to_sleep;
	data->notepme = GET_ARG(NOTEPME);

	data->started_time = philo_time(0);
	
	data->should_stop = 0;
	data->forks = philo_mutex_init(data->number_of_philos);
	if (data->forks == NULL)
		return (0);
	data->routine = philo_routine;

	if (data->notepme)
		data->routine = philo_routine_each_time;
	return (1);
}
