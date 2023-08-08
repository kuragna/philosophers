/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_mutex_destroy.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 15:53:14 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/08 20:10:51 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	philo_mutex_destroy(t_mutex *ptr,  const int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (pthread_mutex_destroy(ptr + i) == -1)
			return (-1);
		i += 1;
	}
	free(ptr);
	return (0);
}
