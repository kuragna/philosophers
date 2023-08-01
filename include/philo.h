/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:10:44 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/01 17:56:32 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>


#define NUMBER_ARGS (3)
#define GET_ARG(time) ft_atoi(args[time])

#define ERROR(str) printf("Error: %s\n", str) // change it to stderr

enum	e_philo_input
{
	NUMBER_OF_PHILOS = 0,
	T_DIE,
	T_EAT,
	T_SLEEP,
	NOTEPME,
	T_THINK,
};

typedef struct s_data
{
	int	number_of_philos;
	int	time_to_eat;
	int	time_to_sleep;
	int	time_to_think;
	int	time_to_die;
	int notepme;
	int	*forks;
	pthread_mutex_t	*mutex_forks;
}	t_data;

typedef struct s_philo
{	
	int			id;
	int 		right_hand;
	int 		left_hand;
	pthread_mutex_t	right_fork;
	pthread_mutex_t	left_fork;
	t_data		*data;
	pthread_t	thread;
}	t_philo;


int				*philo_fill(int n, size_t size);
int				philo_join(t_philo *philos, const int number_of_philos);
void			philo_eat(t_philo *philo);
void			philo_die(t_philo *philo);
void			philo_think(t_philo *philo);
void			philo_sleep(t_philo *philo);
time_t			philo_time(void);

pthread_mutex_t	*philo_mutex_init(const int size);
t_data			*philo_set_data(char **args);
int				ft_atoi(const char *str);
int				ft_isdigit(int c);
int				philo_parse_input(char **args);
void			philo_fokrs_status(t_philo *philo);

void			find_leaks(void);

#endif // PHILO_H
