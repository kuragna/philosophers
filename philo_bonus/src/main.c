/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 18:46:19 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/17 18:52:00 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

// TODO: Fix print nothing after die
// TODO: Fix 200 philosophers

static void	philo_routine(t_philo *philo)
{
	sem_wait(philo->data->forks);
	philo_log(philo, "has taken a fork");
	if (philo->data->philo_number == 1)
		philo_usleep(1, philo);
	sem_wait(philo->data->forks);
	philo_log(philo, "has taken a fork");
	philo_log(philo, "is eating");
	philo_usleep(philo->data->time_to_eat, philo);
	philo->last_meal = philo_get_time() - philo->data->started_time;
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	philo_log(philo, "is sleeping");
	philo_usleep(philo->data->time_to_sleep, philo);
	philo_log(philo, "is thinking");
	philo->meal_number -= 1;
}

static void	philo_process(t_philo *philo)
{
	philo->meal_number = philo->data->meal_number;
	while (true)
	{
		if (philo->data->meal_number && philo->meal_number == 0)
			exit(EXIT_MEAL);
		philo_routine(philo);
	}
}

static void	philo_is_died(t_philo *philo, const size_t philo_number)
{
	int		status;
	size_t	i;
	size_t	exit_count;

	i = 0;
	exit_count = 0;
	while (true)
	{
		waitpid(-1, &status, 0);
		if (WEXITSTATUS(status) == EXIT_MEAL)
		{
			exit_count += 1;
			if (exit_count == philo->data->philo_number)
				break ;
		}
		if (WEXITSTATUS(status) == EXIT_DIED)
		{
			while (i < philo_number)
				kill(philo[i++].pid, SIGKILL);
			break ;
		}
	}
}

static bool	philo_init(t_data *data)
{
	t_philo	*philo;
	size_t	i;

	philo = malloc(sizeof(*philo) * data->philo_number);
	if (philo == NULL)
		philo_destroy(data, "Error: Failed to allocate memory\n");
	memset(philo, 0, sizeof(*philo) * data->philo_number);
	i = -1;
	data->started_time = philo_get_time();
	while (++i < data->philo_number)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].pid = fork();
		if (philo[i].pid == -1)
		{
			free(philo);
			philo_destroy(data, "Error: Could not create child process\n");
		}
		if (philo[i].pid == 0)
			philo_process(philo + i);
	}
	philo_is_died(philo, data->philo_number);
	free(philo);
	return (true);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!philo_get_data(&data, argv + 1, argc - 1))
		return (1);
	philo_init(&data);
	return (0);
}
