/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 20:46:16 by aabourri          #+#    #+#             */
/*   Updated: 2023/08/11 14:13:14 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

#define RIGHT philo->id % philo->data->number_of_philos
#define LEFT philo->id - 1


void	philo_put_right_fork(t_philo *philo);
void	philo_pick_left_fork(t_philo *philo);
void	philo_pick_right_fork(t_philo *philo);

// NOTE: philosephers can't pick up two fork at the same time.
// NOTE: they can't pick up a fork if already with another philosopher.
// NOTE: try to pick up a fork if there are two or by using odd and even
// NOTE: number of philosphers can eat at same time N/2
// NOTE: make sure program works if optional argument is 0
// TODO: check usleep for errors.
// TODO: don't allocate memory if number_of_philos is 0
// NOTE: how can one philospher die if he trying pocked up left fork

void	philo_pick_fork(int id, int idx, int *hand, t_data *data)
{
	if (*hand)
		return ;
	pthread_mutex_lock(data->forks + idx);
	*hand = 1;
	printf("%ld %d has taken a fork\n", philo_time() - data->started_time, id);
}

void	philo_eat(t_philo *philo)
{
	const int right = philo->id % philo->data->number_of_philos;
	const int left = philo->id - 1;

	if (!philo->right_hand || !philo->left_hand)
		return ;

	printf("%ld %d is eating\n", philo_time() - philo->data->started_time, philo->id);
	usleep(philo->data->time_to_eat);

	pthread_mutex_unlock(philo->data->forks + right);
	philo->right_hand = 0;

	pthread_mutex_unlock(philo->data->forks + left);
	philo->left_hand = 0;
}

void	philo_sleep_think(t_philo *philo)
{
	printf("%ld %d is sleeping\n", philo_time() - philo->data->started_time, philo->id);
	usleep(philo->data->time_to_sleep);
	printf("%ld %d is thinking\n", philo_time() - philo->data->started_time, philo->id);
	usleep(philo->data->time_to_think);
}


void	*philo_routine(void *arg)
{
	t_philo	*philo = arg;

	while (1)
	{
		if (philo->id % 2 != 0)
		{
			philo_pick_fork(philo->id, RIGHT, &philo->right_hand, philo->data);
			philo_pick_fork(philo->id, LEFT, &philo->left_hand, philo->data);
			philo_eat(philo);
			philo_sleep_think(philo);
		}
	}
	return NULL;
}

int	philo_init(char **args)
{
	size_t	i;
	t_data	data;
	t_philo *philos;

	philo_get_data(&data, args);

	philos = malloc(sizeof(*philos) * data.number_of_philos);
	if (philos == NULL)
		return (0);

	i = 0;
	while (i < data.number_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].data = &data;
		philos[i].right_hand = 0;
		philos[i].left_hand = 0;

		pthread_create(&philos[i].thread, NULL, (void*)philo_routine, philos + i); // check for errors
		i++;
	}
	if (philo_join(philos, data.number_of_philos) == -1)
		return (0);
	return (1);
}

int main(int argc, char **argv)
{
	const int err = philo_parse_input(argv + 1);

	if (err)
	{
		if (--argc == 4 || argc == 5)
			philo_init(argv + 1); // without optional
		else
			printf("Error: Invalid arguments\n");
	}
	else
		printf("Error: Invalid arguments\n");
	return (0);
}