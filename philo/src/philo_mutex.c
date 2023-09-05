/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:37:13 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/04 18:28:13 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	philo_mutex_init(t_data *data)
{
	int		err;
	size_t	i;

	i = 0;
	if (pthread_mutex_init(&data->log_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&data->eat_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&data->meal_mutex, NULL) != 0)
		return (false);
	while (i < data->philo_number)
	{
		err = pthread_mutex_init(data->forks + i, NULL);
		if (err != 0)
			return (false);
		i += 1;
	}
	return (true);
}
