/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aechaoub <aechaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:59:12 by aechaoub          #+#    #+#             */
/*   Updated: 2023/01/25 15:31:12 by aechaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->eating);
	printf("%ld %d has taken a fork\n", gettime() - data->time_start,
		philo->id);
	pthread_mutex_lock(philo->right_fork);
	philo->last_meal = gettime();
	printf("%ld %d has taken a fork\n", gettime() - data->time_start,
		philo->id);
	printf("%ld %d is eating\n", gettime() - data->time_start, philo->id);
	pthread_mutex_unlock(philo->eating);
	sleep_well(data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philo_func(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	int		nbrofmeal;

	nbrofmeal = 0;
	philo = (t_philo *)arg;
	data = (t_data *)philo->data;
	if (philo->id % 2 != 0)
		usleep(500);
	while (1)
	{
		philo_eat(philo, data);
		nbrofmeal++;
		if (nbrofmeal == data->what_enough)
			data->who_complt++;
		pthread_mutex_lock(philo->eating);
		printf("%ld %d is sleeping\n", gettime() - data->time_start, philo->id);
		pthread_mutex_unlock(philo->eating);
		sleep_well(data->time_to_sleep);
		printf("%ld %d is thinking\n", gettime() - data->time_start, philo->id);
	}
	return (0);
}

int	check_death(t_data *data)
{
	int	i;

	i = 0;
	while (1)
	{
		i = 0;
		while (i < data->nbm_philos)
		{
			pthread_mutex_lock(data->philos[i].eating);
			if (gettime() - data->philos[i].last_meal > data->time_to_die)
			{
				printf("%ld %d died\n", gettime() - data->time_start, i);
				return (1);
			}
			pthread_mutex_unlock(data->philos[i].eating);
			if (data->who_complt == data->nbm_philos)
			{
				printf("%ld  Everyone has their meals \n", gettime()
					- data->time_start);
				return (1);
			}
			i++;
		}
		usleep(50);
	}
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nbm_philos);
	if (!data->philos)
		return (0);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbm_philos);
	if (!data->forks)
		return (0);
	data->eat = malloc(sizeof(pthread_mutex_t) * data->nbm_philos);
	if (!data->eat)
		return (0);
	i = 0;
	while (i < data->nbm_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->eat[i], NULL);
		i++;
	}
	i = 0;
	while (i < data->nbm_philos)
	{
		data->philos[i].data = data;
		data->philos[i].id = i;
		data->philos[i].eating = &data->eat[i];
		data->philos[i].last_meal = gettime();
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nbm_philos];
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_data		data;
	pthread_t	thread;
	int			i;

	if (ac != 6 && ac != 5)
		return (0);
	i = 0;
	if (!take_args(ac, av, &data))
		return (0);
	if (!init_philos(&data))
		return (0);
	data.time_start = gettime();
	i = 0;
	while (i < data.nbm_philos)
		pthread_create(&thread, NULL, philo_func, &data.philos[i++]);
	if (check_death(&data))
		return (0);
	pthread_join(thread, NULL);
}
