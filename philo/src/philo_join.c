/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_join.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:41:01 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/03 13:51:17 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	philo_join(t_philo *philo)
{
	size_t	i;
	int		err;

	i = 0;
	while (i < philo->data->philo_number)
	{
		err = pthread_join(philo[i].thread, NULL);
		if (err != 0)
			return (philo_error("Error: Failed to join thread\n"));
		i += 1;
	}
	return (true);
}
