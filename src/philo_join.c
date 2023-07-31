/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_join.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:41:01 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/31 17:06:55 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	philo_join(t_philo *philo, const int number_of_philos)
{
	int	i;

	i = 0;
	while (i < number_of_philos)
	{
		if (pthread_join(philo[i].thread, NULL) == -1)
			return (-1);
		i += 1;
	}
	return (1);
}
