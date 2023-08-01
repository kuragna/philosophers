/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:37:13 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/31 20:27:34 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

pthread_mutex_t	*philo_mutex_init(const int size)
{
	int	i;
	pthread_mutex_t	*mutexes;

	i = 0;
	mutexes = malloc(sizeof(*mutexes) * size);
	if (mutexes == NULL)
		return (NULL);
	while (i < size)
	{
		if (pthread_mutex_init(mutexes + i, NULL) == -1)
			return (NULL);
		i += 1;
	}
	return (mutexes);
}
