/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:14:08 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/16 20:35:10 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

// NOTE:
// 		eat: no sleep nor think
// 		sleep: no eat nor think
// 		think: no eat nor sleep
// NOTE: number_of_philos = number_of_forks
// NOTE: program how many philosophers could accept?
// NOTE: philosophers should avoid dying.

/* TODO: number_of_times_each_philosopher_must_eat (optional argument): 
If all philosophers have eaten at least number_of_times_each_philosopher_must_eat 
times, the simulation stops. */

// TODO; check every argument if has value <= 0
// TODO: each philosopher should be a thread
// TODO: change errors to stderr
// TODO: check if pthread's function failed.

typedef struct s_philo
{
	int	number_of_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	optional;
}	t_philo;

void	find_leaks()
{
	system("leaks -q philo");
}

int	print_state(int	philo)
{
	struct timeval	tv;
	time_t	current_time;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	current_time = tv.tv_sec;
	printf("%zu ms %d has taken a fork\n", current_time, philo);
	printf("%zu ms %d is eating\n", current_time, philo);
	printf("%zu ms %d is sleeping\n", current_time, philo);
	printf("%zu ms %d is thinking\n", current_time, philo);
	printf("%zu ms %d died\n", current_time, philo);
	return (0);
}

void	print_value(t_philo philo)
{
	printf("--------------------------------\n");
	printf("number_of_philos: %d\n", philo.number_of_philos);
	printf("time_to_die: %d\n", philo.time_to_die);
	printf("time_to_eat: %d\n", philo.time_to_eat);
	printf("time_to_sleep: %d\n", philo.time_to_sleep);
	printf("number_of_times_each_philo: %d\n", philo.optional);
	printf("--------------------------------\n");
}

t_philo	*philo_init(char **args)
{
	t_philo *philo;

	philo = malloc(sizeof(*philo));
	if (!philo)
		return (NULL);
	philo->number_of_philos = ft_atoi(args[NUMBER_OF_PHILOS]);
	philo->time_to_die = ft_atoi(args[TIME_TO_DIE]);
	philo->time_to_eat = ft_atoi(args[TIME_TO_EAT]);
	philo->time_to_sleep = ft_atoi(args[TIME_TO_SLEEP]);
	philo->optional = ft_atoi(args[OPTIONAL]);
	return (philo);
}

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

//		0		1			 2			 3				4		   5
// ./philo nb_of_philos  time_to_die time_to_eat time_to_sleep [optional]


// time_to_sleep: The time a philosoper spend sleeping
// time_to_eat: The time a philosoper spend eating, need to hold 2 forks.
// range(1, number_of_philosopers)

int main2(int argc, char **argv)
{
	t_philo *philo;

	if (!parse_input(argv + 1))
	{
		printf("Error: Invalid argument\n");
		return (1);
	}
	philo = philo_init(argv + 1);
	if ((argc - 1) == 4) // philo without optional
	{
		print_value(*philo);
		printf("philo without optional\n");
	}
	else if ((argc - 1) == 5) // philo with optional
	{
		print_value(*philo);
		printf("philo with optional\n");
	}
	else
	{
		printf("Usage: ./philo number_of_philos time_to_die time_to_eat time_to_sleep [optional]\n");
		return (1);
	}

	return (0);
}


void	pick_right_fork(int philo_id)
{
	printf("philo's %d picked up right fork\n", philo_id);
}

void	pick_left_fork(int philo_id)
{
	printf("philo's %d picked up left fork\n", philo_id);
}

void	put_left_fork(int philo_id)
{
	printf("philo's %d put down left fork\n", philo_id);
}

void	put_right_fork(int philo_id)
{
	printf("philo's %d put down right fork\n", philo_id);
}

typedef struct
{
	int	philo_id;
	int	time_to_sleep;
	int	time_to_eat;
	int	left_fork;
	int	right_fork;	
	int	is_eat;
	int	is_think;
	int	is_sleep;
} Philo;

void	philo_eat(int time_to_eat)
{
	(void)time_to_eat;
	printf("Philo start eat\n");
	sleep(1);
	printf("Philo finished eat\n");
}

void	philo_sleep(int time_to_sleep)
{
	(void)time_to_sleep;	
	printf("Philo start sleep\n");
	sleep(1);
	printf("Philo woke up\n");
}

void	*start_routine(void *arg)
{
	Philo philo = *(Philo *)arg;
	while (1)
	{
		philo_eat(philo.time_to_eat);
		philo_sleep(philo.time_to_sleep);
	}
	return (NULL);
}


int main(void)
{
	Philo philo = {0};
	pthread_t	thread;

	philo.time_to_sleep = 6427;
	philo.time_to_eat = 5638;
	pthread_create(&thread, NULL, start_routine, &philo);
	pthread_join(thread, NULL);
	return 0;
}

