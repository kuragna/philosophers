/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_eatch_time.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 14:58:41 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/11 13:29:25 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// simulation stops when philosopher died of notepme is 0.

void	*philo_routine_each_time(void *arg)
{
	t_philo *philo;
	size_t	i;
	
	i = 0;
	philo = arg;
	while (i < philo->data->notepme) // add condition if philosopher dies
	{
		printf("Not impelmented\n");
		i += 1;
	}
	return (NULL);
}
