/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:40:05 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/31 17:18:59 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

t_data *set_data(char **args)
{
	int	i;
	t_data *data;

	i = 0;
	data = malloc(sizeof(*data));
	if (data == NULL)
		return (NULL);

	data->number_of_philos = GET_ARG(NUMBER_OF_PHILOS);
	data->time_to_eat = GET_ARG(T_EAT);
	data->time_to_sleep = GET_ARG(T_SLEEP);
	data->time_to_die = GET_ARG(T_DIE);
	data->time_to_think = 4242;

	data->forks = philo_fill(1, data->number_of_philos);
	if (data->forks == NULL)
		return (NULL);

	data->mutex_forks = mutex_init(data->number_of_philos);
	if (data->mutex_forks == NULL)
		return (NULL);

	return (data);
}
