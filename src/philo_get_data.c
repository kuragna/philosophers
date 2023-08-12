/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:40:05 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/12 16:00:53 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	philo_get_data(t_data *data, char **args)
{
	data->number_of_philos = GET_ARG(NUMBER_OF_PHILOS);
	data->time_to_eat = GET_ARG(T_EAT);
	data->time_to_sleep = GET_ARG(T_SLEEP);
	data->time_to_die = GET_ARG(T_DIE);
	data->time_to_think = (data->time_to_eat * 2) - data->time_to_sleep;
	data->notepme = GET_ARG(NOTEPME);

	data->started_time = philo_time(0);
	
	data->should_stop = 0;
	data->forks = philo_mutex_init(data->number_of_philos);
	if (data->forks == NULL)
		return (1);
	data->routine = philo_routine;

	if (data->notepme)
		data->routine = philo_routine_each_time;
	return (0);
}

