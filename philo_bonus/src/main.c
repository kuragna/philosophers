#include "../include/philo_bonus.h"

#include <signal.h>
// TODO: make sure to unlink and close semaphore before kill process manually
// TODO: fix more than 123 philosophers.
// NOTE: 1 - cant access from outside

#define LOG(str) philo_log(philo, str)

#define EXIT_MEAL 42

long long	philo_get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	philo_usleep(long long time)
{
	const long long	start = philo_get_time() + time;

	while (philo_get_time() < start)
		usleep(100);
}

void	philo_log(t_philo *philo, const char *str)
{
	const long long curr_time = philo_get_time() - philo->data->started_time;

// 	if (philo->log)
// 	{
		printf("%lld %d %s\n", curr_time, philo->id, str);
// 	}
}

void	philo_routine(t_philo *philo)
{
	sem_wait(philo->data->forks);
	LOG("has taken a fork");
	sem_wait(philo->data->forks);
	LOG("has taken a fork");
	LOG("is eating");
	philo_usleep(philo->data->time_to_eat);
	philo->last_meal = philo_get_time() - philo->data->started_time;
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	LOG("is sleeping");
	philo_usleep(philo->data->time_to_sleep);
	LOG("is thinking");
	philo->meal_number += 1;
}

void	*routine(void *arg)
{
	t_philo	*philo = arg;

	while (true)
	{
// 		philo->start_time = philo_get_time();
		if (philo->data->meal_number && philo->meal_number == philo->data->meal_number)
		{
			philo->exit_flag = true;
			return (NULL);
		}
		philo_routine(philo);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!philo_get_data(&data, argv + 1, argc - 1))
		return (1);
	philo_init(&data);
	sem_close(data.forks);
	sem_unlink(data.sem_name);
	return (0);
}


void	philo_thread(t_philo *philo)
{
	int			err;
// 	long long	last_meal;
	long long	die_time;

	philo->start_time = philo_get_time();
	err = pthread_create(&philo->thread, NULL, routine, philo);
	if (err != 0)
	{
		philo_error("Error: Could not create thread\n");
		exit(EXIT_FAILURE);
	}
	while (true)
	{
		die_time = (philo_get_time() - philo->data->started_time) - philo->last_meal;
		if (die_time >= (long long)philo->data->time_to_die)
		{
			sem_wait(philo->data->death);
			printf("%lld %d died\n", die_time, philo->id);
			exit(EXIT_SUCCESS);
		}
		if (philo->exit_flag)
		{
			exit(EXIT_MEAL);
		}
	}
	err = pthread_join(philo->thread, NULL);
	if (err != 0)
	{
		philo_error("Error: Could not join thread\n");
		exit(EXIT_FAILURE);
	}
}

// TODO: check memory leaks
// TODO: fix timestamp
// TODO: fix data race
// TODO: make processes start at the same time

void	philo_death(t_philo *philo);

bool	philo_init(t_data *data)
{
	t_philo	*philo;
	size_t	i;
	int		status;

	philo = malloc(sizeof(*philo) * data->philo_number);
	if (philo == NULL)
	{
		sem_unlink(data->sem_name); // TODO: check for error
		sem_close(data->forks);
		philo_error("Error: Failed to allocate memory\n");
	}
	i = -1;
	data->started_time = philo_get_time();
	while (++i < data->philo_number)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].meal_number = 0;
		philo[i].pid = fork();
		philo[i].exit_flag = false;
		if (philo[i].pid == -1)
		{
			sem_unlink(data->sem_name);
			sem_close(data->forks);
			philo_error("Error: Could not create a process\n");
			exit(EXIT_FAILURE);
		}
		if (philo[i].pid == 0)
			philo_thread(philo + i);
	}
	size_t	exit_count = 0;
	while (true)
	{
		for (size_t k = 0; k < data->philo_number; k++)
		{
			waitpid((pid_t)-1, &status, 0);
			if (WEXITSTATUS(status) == EXIT_MEAL)
			{
				exit_count += 1;
			}
			if (exit_count == data->philo_number)
			{
				return (true);
			}
		}
	}
#if 0
		waitpid((pid_t)-1, &status, 0);
		if (WEXITSTATUS(status) == EXIT_MEAL)
		{
			count += 1;
			printf("count: %ld\n", count);
			if (count == data->philo_number)
				return (true);
		}
		else if (WIFEXITED(status))
		{
			printf("wstatus: %d\n", WEXITSTATUS(status));
			i = 0;
			while (i < data->philo_number)
			{
				kill(philo[i].pid, SIGKILL);
				i += 1;
			}
			return (true);
		}
#endif
	return (true);	
}

// 1 800 200 200
// a philo should die
// 4 310 200 100
// a philo should die
// 4 200 205 200
// a philo should die
// 5 800 200 200 7
// no one should die, simulation should stop after 7 eats
// 4 410 200 200 10
// no one should die, simulation should stop after 10 eats
// -5 600 200 200
// should error and not run (no crashing)
// 4 -5 200 200
// should error and not run (no crashing)
// 4 600 -5 200
// should error and not run (no crashing)
// 4 600 200 -5
// should error and not run (no crashing)
// 4 600 200 200 -5
// should error and not run (no crashing)
//--------------------------------------------
// 5 800 200 200
// no one should die
// 5 600 150 150
// no one should die
// 4 410 200 200
// no one should die
// 100 800 200 200
// no one should die
// 105 800 200 200
// no one should die
// 200 800 200 200
// no one should die