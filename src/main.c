/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:14:08 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/12 16:34:25 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <string.h>
#include <unistd.h>

#define PRINT_PHILO() print_philo(philo)
// number_of_philosophers time_to_die time_to_eat
// time_to_sleep
// [number_of_times_each_philosopher_must_eat]
typedef enum e_philo
{
	NUMBER_OF_PHILOS = 0,
	TIME_TO_DIE,
	TIME_TO_EAT,
	TIME_TO_SLEEP,
	NUMBER_OF_TIMES_EACH_PHILO,
}	t_enum_philo;

typedef struct s_philo
{
	// NOTE: shoud be greater than 0
	// NOTE: number of forks also
	int			number_of_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			number_of_times_each_philo; // optional argument
	void		(*funct_ptr)();
}	t_philo;

void	print_philo(t_philo philo)
{
	printf("--------------------------------\n");
	printf("number_of_philos: %d\n", philo.number_of_philos);
	printf("time_to_die: %d\n", philo.time_to_die);
	printf("time_to_eat: %d\n", philo.time_to_eat);
	printf("time_to_sleep: %d\n", philo.time_to_sleep);
	printf("number_of_times_each_philo: %d\n", philo.number_of_times_each_philo);
	printf("--------------------------------\n");
}

t_philo	get_input(char **args)
{
	t_philo	philo;
	
	philo.number_of_philos = ft_atoi(args[NUMBER_OF_PHILOS]);
	philo.time_to_die = ft_atoi(args[TIME_TO_DIE]);
	philo.time_to_eat = ft_atoi(args[TIME_TO_EAT]);
	philo.time_to_sleep = ft_atoi(args[TIME_TO_SLEEP]);
	philo.number_of_times_each_philo  = ft_atoi(args[NUMBER_OF_TIMES_EACH_PHILO]);
	return philo;
}


// NOTE:
// 		eat: no sleep and think
// 		sleep: no eat and think
// 		think: no eat and sleep
// NOTE: number_of_philos = number_of_forks

int	print_state(int	philo)
{
	struct timeval	tv;
	time_t			tv_sec;

	// NOTE: x is number of philosopher
	// NOTE: function maybe failed
	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	tv_sec = (tv.tv_sec * 1000);
	printf("%zu ms %d has taken a fork\n", tv_sec, philo);
	printf("%zu ms %d is eating\n", tv_sec, philo);
	printf("%zu ms %d is sleeping\n", tv_sec, philo);
	printf("%zu ms %d is thinking\n", tv_sec, philo);
	printf("%zu ms %d died\n", tv_sec, philo);
	return (0);
}

int main()
{
	time_t	current_time = 
	return 0;
}

int main2(int argc, char **argv)
{
	(void)argv;
	if (argc != 6)
	{
		printf("Error: %s\n", strerror(22));
		return (1);
	}
	t_philo	philo = get_input(argv + 1);
	if (philo.number_of_philos <= 0)
	{
		printf("Error: Must be at least 1 philosophers\n");
		return (1);
	}
	int err = print_state(1);
	if (err == -1)
	{
		printf("Error: %s\n", strerror(err));
		return (1);
	}
	return (0);
}
