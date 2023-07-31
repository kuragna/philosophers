/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:42:05 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/31 13:42:06 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	*philo_fill(int n, size_t size)
{
	size_t	i;
	int	*ptr;

	i = 0;
	ptr = malloc(sizeof(*ptr) * size);
	if (ptr == NULL)
		return (NULL);
	while (i < size)
	{
		*(ptr + i) = n;
		i++;
	}
	return (ptr);
}
