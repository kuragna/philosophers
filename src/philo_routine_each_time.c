/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_eatch_time.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 14:58:41 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/08 17:29:20 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// simulation stops when philosopher died of notepme is 0.

void	*philo_routine_each_time(void *arg)
{
	t_philo *philo = arg;
	int	i;

	i = 0;
	while (i < philo->data->notepme && !philo->data->stop)
	{
		printf("Not impelmented\n");
		i += 1;
	}
	return (NULL);
}
