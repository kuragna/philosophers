/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 20:37:27 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/11 17:01:38 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

static bool	philo_check_input(char **args, const int args_size)
{
	char	*arg;
	int		i;

	if (args_size < 4 || args_size > 5)
	{
		philo_error("Usage: ./philo number_of_philos time_to_die time_to_eat");
		return (philo_error(" time_to_sleep [number_of_meals]\n"));
	}
	i = 0;
	while (args[i] != NULL)
	{
		arg = args[i];
		while (*arg != '\0')
		{
			if (!(48 <= *arg && 57 >= *arg))
			{
				return (philo_error("Error: Invalid arguments\n"));
			}
			arg++;
		}
		i += 1;
	}
	return (true);
}

static bool	philo_sem_init(sem_t **sem, const char *sem_name, const size_t sem_size)
{
	sem_unlink(sem_name);
	*sem = sem_open(sem_name, O_CREAT, 0644, sem_size);
	if (*sem == SEM_FAILED)
	{
		sem_unlink(sem_name);
		return (philo_error("Error: Could not initial semaphore\n"));
	}
	return (true);
}

// TODO: Maybe need to unlink semaphore name.
bool	philo_get_data(t_data *data, char **args, const int args_size)
{
	if (!philo_check_input(args, args_size))
	{
		sem_unlink(data->sem_name);
		return (false);
	}
	data->philo_number = ft_atoi(args[PHILO_NUMBER]);
	data->time_to_die = ft_atoi(args[T_DIE]);
	data->time_to_eat = ft_atoi(args[T_EAT]);
	data->time_to_sleep = ft_atoi(args[T_SLEEP]);
	if ((int)data->philo_number <= 0 || (int)data->time_to_die <= 0
		|| (int)data->time_to_sleep <= 0 || (int)data->time_to_eat <= 0)
		return (philo_error("Error: Invalid arguments\n"));
	data->meal_number = ft_atoi(args[MEAL_NUMBER]);
	data->log = true;
	data->sem_name = "/philo";
	data->death_sem_name = "/death";
	if (!philo_sem_init(&data->forks, data->sem_name, data->philo_number))
		return (false);
	if (!philo_sem_init(&data->death, data->death_sem_name, 1))
		return (false);

	printf("forks: %p\n", data->forks);
	printf("death: %p\n", data->death);
	return (true);
}

bool	philo_error(const char *str)
{
	while (*str != '\0')
	{
		write(2, str++, 1);
	}
	return (false);
}
