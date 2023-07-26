/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <aabourri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:14:08 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/26 20:40:57 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <errno.h>
#include <limits.h>

#define GET_ARG(time) ft_atoi(args[time])
#define JOIN(thread) pthread_join(thread, NULL)
#define FORKS_STATUS() philo_forks_status(philo)
#define PRINT_HANDS() printf("ID: %d | hands[%d, %d]\n", \
		philo->id,\
		philo->right_hand, \
		philo->left_hand)


#define P_THINKING 1
#define P_EATING 2
#define P_SLEEPING 3
int	philo_get_fork(int *forks, int index, pthread_mutex_t *mutex);

enum	e_philo_time
{
	T_DIE = 0,
	T_EAT,
	T_SLEEP,
	T_THINK,
};

typedef struct s_philo
{	
	int			id; // 4
	int			right_hand; // 4
	int			left_hand; // 4
	int			number_of_philos; // 4
	int			status; // 4
	int			*forks; // 8
	int			*times; // 8
	pthread_mutex_t	*mutex_ptr; // 8
	pthread_t	thread; // 8
}	t_philo; // 52 != 56

typedef struct s_data
{
	int	times[4];
	int	*forks;
	pthread_mutex_t	mutex;
	t_philo *philos;
}	t_data;

void	find_leaks()
{
	system("leaks -q philo");
}

void	philo_forks_status(t_philo *philo)
{
	pthread_mutex_lock(philo->mutex_ptr);
	printf("------------ forks status ------------\n");
	printf("{ ");
	for (int i = 0; i < philo->number_of_philos; i++)
	{
		if (!philo->forks[i])
			printf("HAND");
		else
			printf("TABLE");
		if (i != philo->number_of_philos - 1)
			printf(", ");
	}
	printf(" }\n");
	printf("------------ forks status ------------\n");
	pthread_mutex_unlock(philo->mutex_ptr);
}

time_t	philo_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec);
}

int	*philo_fill(int *ptr, int n, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		*(ptr + i) = n;
		i++;
	}
	return (ptr);
}

int	philo_parse_input(char **args)
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




void	philo_sleep(t_philo *philo)
{
	printf("%zu %d is sleeping\n", philo_time(), philo->id);
	usleep(philo->times[T_SLEEP]);
}


// TODO: philosephers can't pick up two fork at the same time.
// NOTE: he can't pick up a fork if already with another philosopher.
// NOTE: try to pick up a fork if there are two or by using odd and even



// NOTE: 1 means fork on the table
// NOTE: 0 means philosopher has taken it


void	philo_put_right_fork(t_philo *philo)
{
	const int index = philo->id % philo->number_of_philos;

	if (!philo->right_hand)
		return ;

	pthread_mutex_lock(philo->mutex_ptr);
	philo->forks[index] = philo->right_hand;
	philo->right_hand = 0;
// 	printf("%zu %d has put right fork\n", philo_time(), philo->id);
	pthread_mutex_unlock(philo->mutex_ptr);
}

void	philo_put_left_fork(t_philo *philo)
{
	const int index = philo->id - 1;
	
	if (!philo->left_hand)
		return ;

	pthread_mutex_lock(philo->mutex_ptr);
	philo->forks[index] = philo->left_hand;
	philo->left_hand = 0;
// 	printf("%zu %d has put left fork\n", philo_time(), philo->id);
	pthread_mutex_unlock(philo->mutex_ptr);
}



void	philo_pick_right_fork(t_philo *philo)
{
// 	const time_t time = philo_time();	
	const int index = philo->id % philo->number_of_philos;
	const int fork = philo_get_fork(philo->forks, index, philo->mutex_ptr);

	if (philo->right_hand || !fork)
		return ;
	pthread_mutex_lock(philo->mutex_ptr);
	philo->right_hand = philo->forks[index];
	philo->forks[index] = 0;
// 	printf("%zu %d has taken a right fork\n", time, philo->id);
	pthread_mutex_unlock(philo->mutex_ptr);
}

void	philo_think(t_philo *philo)
{
	if (philo->right_hand == 1 && philo->left_hand == 0)
		philo_put_right_fork(philo);
	else if (philo->right_hand == 0 && philo->left_hand == 1)
		philo_put_left_fork(philo);

	if (philo->right_hand && philo->left_hand)
		return ;
// 	printf("%zu %d is thinking\n", philo_time(), philo->id);
	usleep(philo->times[T_THINK]);
}

void	philo_pick_left_fork(t_philo *philo)
{
// 	const time_t time = philo_time();
	const int index = philo->id - 1;
	const int fork = philo_get_fork(philo->forks, index, philo->mutex_ptr);

	if (philo->number_of_philos == 1)
		return ;
	if (philo->id % 2 != 1 && philo->right_hand != 1)
		return ;
	if (philo->left_hand || !fork)
		return ;
	pthread_mutex_lock(philo->mutex_ptr);
	philo->left_hand = philo->forks[index];
	philo->forks[index] = 0;
// 	printf("%zu %d has taken a left fork\n", time, philo->id);
	pthread_mutex_unlock(philo->mutex_ptr);
}


void	philo_eat(t_philo *philo)
{
	if (!philo->right_hand || !philo->left_hand)
		return ;
// 	printf("%zu %d is eating\n", philo_time(), philo->id);
	usleep(philo->times[T_EAT]);
	philo_put_right_fork(philo);
	philo_put_left_fork(philo);
}

int	philo_get_fork(int *forks, int index, pthread_mutex_t *mutex)
{
	int	fork;

	pthread_mutex_lock(mutex);
	fork = forks[index];
	pthread_mutex_unlock(mutex);
	return (fork);
}

void	*philo_routine(t_philo *philo)
{
	while (1)
	{
		philo_pick_right_fork(philo);
		philo_pick_left_fork(philo);
		philo_think(philo);
		if (philo->right_hand && philo->left_hand)
		{
			philo_eat(philo);
			printf("ID: %d\n", philo->id);
		}

// 		printf("ID: %d | hands[%d, %d]\n", philo->id, philo->right_hand, philo->left_hand);
		philo_eat(philo);
		sleep(1);
	}
	return NULL;
}

// NOTE: number of philosphers can eat at same time N/2

t_data	*philo_init(char **args, const int number_of_philos)
{
	int	i;
	t_data	*data;

	i = -1;
	data = malloc(sizeof(*data));

	data->times[T_DIE] = GET_ARG(T_DIE);
	data->times[T_EAT] = GET_ARG(T_EAT);
	data->times[T_SLEEP] = GET_ARG(T_SLEEP);
	data->times[T_THINK] = 4242;

	data->philos = malloc(sizeof(*data->philos) * number_of_philos);
	data->forks = malloc(sizeof(*data->forks) * number_of_philos);

	philo_fill(data->forks, 1, number_of_philos);

	pthread_mutex_init(&data->mutex, NULL);

	while (++i < number_of_philos)
	{
		data->philos[i].id = i + 1;

		data->philos[i].times = data->times;
		data->philos[i].mutex_ptr = &data->mutex;

		data->philos[i].number_of_philos = number_of_philos;
		data->philos[i].forks = data->forks;

		pthread_create(&data->philos[i].thread, NULL, (void*)philo_routine, data->philos + i);
	}
	return (data);
}

int	philo_join(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos->number_of_philos)
	{
		if (JOIN(data->philos[i].thread) == -1)
			return (0);
	}
	return (1);
}
int main(int argc, char **argv)
{
	if (argc != 5 || !philo_parse_input(argv + 1))
	{
		printf("Error: %s\n", strerror(EINVAL));
		return (1);
	}
	int	number_of_philos = ft_atoi(argv[1]);
	t_data *data = philo_init(argv + 2, number_of_philos);
	philo_join(data);
	return (0);
}