/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:14:08 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/18 21:00:15 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// TODO; check every argument if has value <= 0
// TODO: each philosopher should be a thread
// TODO: change errors to stderr
// TODO: check if pthread's function failed.
// TODO: check if input has error.

typedef struct s_data
{
	int	time_to_eat;
	int	time_to_sleep;
	int	time_to_die;
}	t_data;

typedef struct s_philo
{
	int	id;
	int	left_fork;
	int	right_fork;
	t_data data;
	pthread_t thread;
}	t_philo;


void	find_leaks()
{
	system("leaks -q philo");
}

time_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec);
}

// void	print_value(t_data data)
// {
// 	printf("--------------------------------\n");
// 	printf("number_of_datas: %d\n", data.number_of_philos);
// 	printf("time_to_die: %d\n", data.time_to_die);
// 	printf("time_to_eat: %d\n", data.time_to_eat);
// 	printf("time_to_sleep: %d\n", data.time_to_sleep);
// 	printf("number_of_times_each_data: %d\n", data.optional);
// 	printf("--------------------------------\n");
// }


int	parse_input(char **args)
{
	char	*str;
	int		i;

	i = -1;
	while (args[++i] != NULL)
	{
		str = args[i];
		while (*str)
		{
			if (!ft_isdigit(*str))
				return (FALSE);
			str++;
		}
	}
	return (TRUE);
}

void	pick_left_fork()
{
	printf("Not implemented yet\n");
}

void	philo_sleep(int time_to_sleep, int id)
{
	printf("%zu %d is sleeping\n", get_time(), id);
	usleep(time_to_sleep);
}

void	philo_eat(int time_to_eat, int id)
{
	printf("%zu %d is eating\n", get_time(), id);
	usleep(time_to_eat);
}

void	*start_routine(void *arg)
{
	t_philo *philo = (t_philo*)arg;
	while (1)
	{
		philo_eat(philo->data.time_to_eat, philo->id);
		philo_sleep(philo->data.time_to_sleep, philo->id);
	}
	return (NULL);
}

int	philo_join(t_philo *philo, const int number_of_philos)
{
	int	i;

	i = 0;
	while (i < number_of_philos)
	{
		if (pthread_join(philo[i].thread, NULL) == -1)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	philo_create(char **args, const int number_of_philos)
{
	const int	time_to_eat  = ft_atoi(args[TIME_TO_EAT]);
	const int	time_to_sleep = ft_atoi(args[TIME_TO_SLEEP]);
	const int	time_to_die = ft_atoi(args[TIME_TO_DIE]);
	int	i;
	t_philo	*philo;


	i = 0;
	philo = malloc(sizeof(*philo) * number_of_philos);
	if (!philo)
		return (FALSE);
	while (i < number_of_philos)
	{
		philo[i].id = (i + 1);
		philo[i].data.time_to_die = time_to_die;
		philo[i].data.time_to_eat = time_to_eat;
		philo[i].data.time_to_sleep = time_to_sleep;
		if (pthread_create(&philo[i].thread, NULL, start_routine, &philo[i]) == -1)
			return (FALSE);
		i++;
	}
	philo_join(philo, number_of_philos);
	return (TRUE);
}



int main(int argc, char **argv)
{
	if (argc != 5 || !parse_input(argv + 1))
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	const int number_of_philos = ft_atoi(argv[1]);
	philo_create(argv + 1, number_of_philos);
	return (0);
}
