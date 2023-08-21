/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:40:05 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/21 15:43:35 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	philo_parse_input(char **args, const int size)
{
	char	*str;
	int		i;

	i = 0;
	if (size < 4 || size > 5)
		return (0);
	while (args[i] != NULL)
	{
		str = args[i];
		while (*str)
		{
			if (!(48 <= *str && 57 >= *str))
				return (FALSE);
			str++;
		}
		i += 1;
	}
	return (TRUE);
}

int	philo_get_data(t_data *data, char **args, const int size)
{
	if (!philo_parse_input(args, size))
		return (philo_error(NULL, "Error: Invalid arguments"));
	data->number_philos = GET_ARG(NUMBER_OF_PHILOS);
	data->time_to_eat = GET_ARG(T_EAT);
	data->time_to_sleep = GET_ARG(T_SLEEP);
	data->time_to_die = GET_ARG(T_DIE);
	if (!data->number_philos || !data->time_to_die || !data->time_to_sleep
		|| !data->time_to_eat)
		return (philo_error(NULL, "Error: Invalid arguments"));
	data->time_to_think = (data->time_to_eat * 2) - data->time_to_sleep;
	data->notepme = GET_ARG(NOTEPME);
	data->started_time = philo_time(0);
	data->should_stop = FALSE;
	data->death_mutex = philo_mutex_init(data->number_philos);
	if (!data->death_mutex)
		return (philo_error(NULL, "Error: Could not initial mutex"));
	data->forks = philo_mutex_init(data->number_philos);
	if (!data->forks)
		return (philo_error(data->death_mutex, "Error: Could not initial mutex"));
// 	data->routine = philo_routine; // TODO: change it to variable count time each philosopher eats
// // 	if (data->notepme)
// // 		data->routine = philo_routine_each_time;
	return (TRUE);
}
