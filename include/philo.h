/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:10:44 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/12 16:00:13 by aabourri         ###   ########.fr       */
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


#define ARGS 4
#define GET_ARG(time) ft_atoi(args[time])

#define RIGHT philo->id % philo->data->number_of_philos
#define LEFT philo->id - 1

typedef pthread_mutex_t	t_mutex;
typedef void *(*t_func_ptr)(void *);

enum
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
	size_t	number_of_philos;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	size_t	time_to_think;
	size_t	time_to_die;
	size_t	notepme;
	size_t	started_time;
	int		should_stop;
	t_mutex	*forks;
	t_func_ptr routine;
}	t_data;

typedef struct s_philo
{	
	int			id;
	int 		right_hand;
	int 		left_hand;
	int			has_ate;
	size_t		last_meal;
	t_data		*data;
	pthread_t	thread;
}	t_philo;

void	philo_pick_fork(int id, int idx, int *hand, t_data *data);
void	philo_sleep_think(t_philo *philo);

int		philo_get_data(t_data *data, char **argv);
int	  	*philo_fill(int n, size_t size);
int	  	philo_join(t_philo *philos, const int number_of_philos);
// void  	philo_eat(t_philo *philo);
void  	philo_die(t_philo *philo);
void  	philo_think(t_philo *philo);
void  	philo_sleep(t_philo *philo);
void	*philo_routine(void *arg);
void	*philo_routine_each_time(void *arg);
// time_t	philo_time(void);
size_t	philo_time(size_t started_time);

t_mutex	*philo_mutex_init(const int size);
int		ft_atoi(const char *str);
int		philo_parse_input(char **args);
void	philo_fokrs_status(t_philo *philo);

void	find_leaks(void);

#endif // PHILO_H
