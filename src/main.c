/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 13:38:25 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/31 17:23:21 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

#define PRINT_HANDS() printf("ID: %d | hands[%d, %d]\n", \
		philo->id,\
		philo->right_hand, \
		philo->left_hand)

#define PICK_RIGHT() philo_pick_right_fork(philo)
#define PICK_LEFT() philo_pick_left_fork(philo)

#define PUT_RIGHT() philo_put_right_fork(philo)
#define PUT_LEFT() philo_put_left_fork(philo)

#define EAT() philo_eat(philo)
#define THINK() philo_think(philo)
#define SLEEP() philo_sleep(philo)


void	philo_put_left_fork(t_philo *philo);
void	philo_put_right_fork(t_philo *philo);
void	philo_pick_left_fork(t_philo *philo);
void	philo_pick_right_fork(t_philo *philo);

// NOTE: philosephers can't pick up two fork at the same time.
// NOTE: they can't pick up a fork if already with another philosopher.
// NOTE: try to pick up a fork if there are two or by using odd and even
// NOTE: number of philosphers can eat at same time N/2



void	philo_eat(t_philo *philo)
{
	if (!philo->right_hand || !philo->left_hand)
		return ;
	printf("%zu %d is eating\n", philo_time(), philo->id);
	usleep(philo->data->time_to_eat);
	PUT_RIGHT();
	PUT_LEFT(); // sleep after put forks
}

void	philo_sleep(t_philo *philo)
{
	printf("%zu %d is sleeping\n", philo_time(), philo->id);
	usleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	printf("%zu %d is thinking\n", philo_time(), philo->id);
	usleep(philo->data->time_to_think);
}

void	philo_pick_right_fork(t_philo *philo)
{
	int	idx = philo->id % philo->data->number_of_philos;
	int	fork_state;

	pthread_mutex_lock(philo->data->mutex_forks + idx);
	fork_state = philo->data->forks[idx];
	pthread_mutex_unlock(philo->data->mutex_forks + idx);

	if (philo->right_hand)
		return ;
	if (!fork_state)
		return ;

	pthread_mutex_lock(philo->data->mutex_forks + idx);
	philo->right_hand = philo->data->forks[idx];
	philo->data->forks[idx] = 0;
	printf("%zu %d has taken a fork\n", philo_time(), philo->id);
	pthread_mutex_unlock(philo->data->mutex_forks + idx);
}

void	philo_pick_left_fork(t_philo *philo)
{
	int	idx = philo->id - 1;
	int	fork_state;

	pthread_mutex_lock(philo->data->mutex_forks + idx);
	fork_state = philo->data->forks[idx];
	pthread_mutex_unlock(philo->data->mutex_forks + idx);

	if (philo->data->number_of_philos == 1)
		return ;
	if (!fork_state)
		return ;
	if (philo->left_hand)
		return ;

	pthread_mutex_lock(philo->data->mutex_forks + idx);
	philo->left_hand = philo->data->forks[idx];
	philo->data->forks[idx] = 0;
	printf("%zu %d has taken a fork\n", philo_time(), philo->id);
	pthread_mutex_unlock(philo->data->mutex_forks + idx);
}

void	philo_put_left_fork(t_philo *philo)
{
	int	idx = philo->id - 1;

	if (!philo->left_hand)
		return ;

	pthread_mutex_lock(philo->data->mutex_forks + idx);
	philo->data->forks[idx] = philo->left_hand;
	philo->left_hand = 0;
	pthread_mutex_unlock(philo->data->mutex_forks + idx);
}
void	philo_put_right_fork(t_philo *philo)
{
	int	idx = philo->id % philo->data->number_of_philos;

	if (!philo->right_hand)
		return ;
	
	pthread_mutex_lock(philo->data->mutex_forks + idx);
	philo->data->forks[idx] = philo->right_hand;
	philo->right_hand = 0;
	pthread_mutex_unlock(philo->data->mutex_forks + idx);
}

void	*philo_routine(t_philo *philo)
{
	while (1)
	{
		PICK_RIGHT();
		PICK_LEFT();
		sleep(1);
	}
	return NULL;
}

int	philo_init(char **args)
{
	int	i;
	t_data	*data;
	t_philo *philos;

	data = set_data(args);
	if (data == NULL)
		return (0);

	philos = malloc(sizeof(*philos) * data->number_of_philos);
	if (philos == NULL)
		return (0);

	i = 0;
	while (i < data->number_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].right_hand = 0;
		philos[i].left_hand = 0;

		pthread_create(&philos[i].thread, NULL, (void*)philo_routine, philos + i); // check for errors
		i++;
	}
	if (philo_join(philos, data->number_of_philos) == -1)
		return (0);
	return (1);
}

int main(int argc, char **argv)
{
	if (argc != 5 || !philo_parse_input(argv + 1))
	{
		ERROR(strerror(EINVAL));
		return (1);
	}
	philo_init(argv + 1);
	return (0);
}