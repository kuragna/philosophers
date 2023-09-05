#include "../include/philo_bonus.h"

bool	philo_check_input(char **args, const int args_size)
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
		while (*arg)
		{
			if (!(48 <= *arg && 57 >= *arg))
			{
// 				return (philo_error("Error: Invalid arguments\n"));
				printf("Error: Invalid arguments\n");
				return false;
			}
			arg++;
		}
		i += 1;
	}
	return (true);
}

bool	philo_get_data(t_data *data, char **args)
{
	data->philo_number = ft_atoi(args[PHILO_NUMBER]);
	data->time_to_die = ft_atoi(args[T_DIE]);
	data->time_to_eat = ft_atoi(args[T_EAT]);
	data->time_to_sleep = ft_atoi(args[T_SLEEP]);
	if (!data->philo_number || !data->time_to_die || !data->time_to_sleep
		|| !data->time_to_eat)
		return (philo_error("Error: Invalid arguments\n"));
	data->meal_number = ft_atoi(args[MEAL_NUMBER]);
	data->sem_name = "/philo";
	data->forks = sem_open(data->sem_name, O_CREAT, 0644, data->philo_number);
	if (data->forks == SEM_FAILED)
	{
		// TODO: Maybe need to unlink semaphore name.
		return (philo_error("Error: Could not initial semaphore\n"));
	}
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