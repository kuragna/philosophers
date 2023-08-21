/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:10:44 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/21 15:39:38 by aabourri         ###   ########.fr       */
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

#define M_SEC 1000

#define RIGHT philo->id % philo->data->number_of_philos
#define LEFT philo->id - 1

#define TRUE 1
#define FALSE 0

enum e_philo_input
{
	NUMBER_OF_PHILOS = 0,
	T_DIE,
	T_EAT,
	T_SLEEP,
	NOTEPME,
	T_THINK,
};

typedef pthread_mutex_t	t_mutex;
typedef void *(*t_func_ptr)(void *);

typedef struct s_data
{
	size_t	number_philos;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	size_t	time_to_think;
	size_t	time_to_die;
	size_t	started_time;
	int		notepme;
	int		should_stop;
	int		evil;
	t_mutex		should_stop_mutex;
// 	t_mutex		*last_meal_mutex;
	t_mutex		*death_mutex;
	t_mutex		*forks;
	t_func_ptr routine;
}	t_data;

typedef struct s_philo
{	
	int			id;
	int 		right_hand;
	int 		left_hand;
	int			has_ate;
	int			should_stop;
	int			eat_count;
	size_t		last_meal;
	t_data		*data;
	pthread_t	thread;
}	t_philo;

void	philo_pick_fork(int id, int idx, int *hand, t_data *data);
void	philo_sleep_think(t_philo *philo);


int		philo_get_data(t_data *data, char **args, const int n);
int	  	philo_join(t_philo *philos);

int		philo_error(void *ptr, const char *str);

void	philo_eat(t_philo *philo);
void	*philo_routine(void *arg);
void	*philo_routine_each_time(void *arg);

int		philo_reset_mem(t_philo *philo);

size_t	philo_time(size_t started_time);

t_mutex	*philo_mutex_init(const size_t size);
int		philo_mutex_destroy(t_philo *philo);

int		ft_atoi(const char *str);
void	find_leaks(void);

#endif // PHILO_H
