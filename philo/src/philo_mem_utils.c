/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_mem_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:05:34 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/04 15:23:25 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_mem.h"

#include <stdio.h>

int	philo_init_mem(t_mem *mem)
{
	mem->capacity = 10;
	mem->lenght = 0;
	mem->ptr = malloc(sizeof(void *) * mem->capacity);
	if (mem->ptr == NULL)
	{
		return (0);
	}
	return (1);
}

void	philo_push_mem(t_mem *mem, void *ptr)
{
	if (mem->capacity == mem->lenght)
	{
		return ;
	}
	mem->ptr[mem->lenght] = ptr;
	mem->lenght += 1;
}

void	philo_reset_mem(t_mem *mem)
{
	size_t	i;

	i = 0;
	while (i < mem->lenght)
	{
		free(mem->ptr[i]);
		i += 1;
	}
	free(mem->ptr);
}
