/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_parse_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:42:44 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/11 13:08:31 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	philo_parse_input(char **args)
{
	char	*str;
	int		i;

	i = -1;
	while (args[++i] != NULL)
	{
		str = args[i];
		while (*str)
		{
			if (!(48 <= *str && 57 >= *str))
				return (0);
			str++;
		}
	}
	return (1);
}
