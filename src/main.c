/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:14:08 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/08 15:21:44 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
// number_of_philosophers time_to_die time_to_eat
// time_to_sleep
// [number_of_times_each_philosopher_must_eat]
int main(int argc, char **argv)
{
	(void)argv;
	if (argc != 5)
	{
		printf("Error: there is no arguments\n");
		return (1);
	}
	printf("Hello, Philo\n");
	return (0);
}
