/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_join.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:41:01 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/17 14:04:29 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	philo_join(t_philo *philo)
{
	const size_t size = philo->data->number_philos;
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (0);
		i += 1;
	}
	return (1);
}
