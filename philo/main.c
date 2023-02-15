/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aechaoub <aechaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:59:12 by aechaoub          #+#    #+#             */
/*   Updated: 2023/02/15 15:08:55 by aechaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_etape(int expression, long time, int id, void *mutex)
{
	pthread_mutex_lock(mutex);
	if (expression == 1)
		printf("%ld %d %s\n", time, id, "has taken a fork");
	if (expression == 2)
		printf("%ld %d %s\n", time, id, "is eating");
	if (expression == 3)
		printf("%ld %d %s\n", time, id, "is sleeping");
	if (expression == 4)
		printf("%ld %d %s\n", time, id, "is thinking");
	pthread_mutex_unlock(mutex);
}

void	philo_eat(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = (t_data *)philo->data;
	pthread_mutex_lock(philo->left_fork);
	print_etape(1, gettime() - philo->start, philo->id,
		&data->print);
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&data->m_date);
	philo->last_meal = gettime();
	pthread_mutex_unlock(&data->m_date);
	print_etape(1, gettime() - philo->start, philo->id,
		&data->print);
	print_etape(2, gettime() - philo->start, philo->id, &data->print);
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
		usleep(250);
	while (1)
	{
		philo_eat(arg);
		nbrofmeal++;
		if (nbrofmeal == data->what_enough)
		{
			pthread_mutex_lock(&data->m_date);
			data->who_complt++;
			pthread_mutex_unlock(&data->m_date);
		}
		print_etape(3, gettime() - philo->start, philo->id, &data->print);
		sleep_well(data->time_to_sleep);
		print_etape(4, gettime() - philo->start, philo->id, &data->print);
	}
	return (0);
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
	i = -1;
	while (++i < data->nbm_philos)
		pthread_mutex_init(&data->forks[i], NULL);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->m_date, NULL);
	data->time_start = gettime();
	i = -1;
	while (++i < data->nbm_philos)
	{
		data->philos[i].data = data;
		data->philos[i].id = i;
		data->philos[i].start = data->time_start;
		data->philos[i].last_meal = gettime();
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nbm_philos];
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_data		data;
	pthread_t	*thread;
	int			i;

	if (ac != 6 && ac != 5)
		return (0);
	i = 0;
	if (!take_args(ac, av, &data))
		return (0);
	if (!init_philos(&data))
		return (0);
	data.who_complt = 0;
	data.stop = 0;
	thread = malloc(sizeof(pthread_create) * data.nbm_philos);
	i = -1;
	while (++i < data.nbm_philos)
		pthread_create(&thread[i], NULL, philo_func, &data.philos[i]);
	i = -1;
	check_death(&data);
	while (++i < data.nbm_philos)
		pthread_detach(thread[i]);
	free(thread);
	return (0);
}
