/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:37:13 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/19 19:18:16 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

t_mutex	*philo_mutex_init(const size_t size)
{
	size_t	i;
	t_mutex	*mutex;

	i = 0;
	mutex = malloc(sizeof(*mutex) * size);
	if (mutex == NULL)
		return (NULL);
	while (i < size)
	{
		if (pthread_mutex_init(mutex + i, NULL) == -1)
			return (NULL);
		i += 1;
	}
	return (mutex);
}
