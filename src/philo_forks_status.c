/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_fork_status.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:57:21 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/31 13:57:21 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	philo_forks_status(t_philo *philo)
{
// 	pthread_mutex_lock(&philo->data->pick_mutex);
	printf("ID: %d = { ", philo->id);
	for (int i = 0; i < philo->data->number_of_philos; i++)
	{
		if (!philo->data->forks[i])
			printf("HAND");
		else
			printf("TABLE");
		if (i != philo->data->number_of_philos - 1)
			printf(", ");
	}
	printf(" }\n");
// 	pthread_mutex_unlock(&philo->data->pick_mutex);
}
