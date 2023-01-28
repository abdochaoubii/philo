/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aechaoub <aechaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:59:12 by adshatub          #+#    #+#             */
/*   Updated: 2023/01/27 17:40:04 by aechaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_etape(char *str,long time , int id,void *mutex)
{
	// pthread_mutex_lock(mutex);
	pthread_mutex_lock(mutex);

	printf("%ld %d %s\n", time,id,str);
	pthread_mutex_unlock(mutex);

}

void	philo_eat(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	philo = (t_philo *)arg;
	data = (t_data *)philo->data;
	
	pthread_mutex_lock(philo->left_fork);
	print_etape("has taken a fork", gettime() - philo->start,philo->id,&data->print);
	pthread_mutex_lock(philo->right_fork);
	philo->last_meal = gettime();
	print_etape("has taken a fork", gettime() - philo->start,philo->id,&data->print);
	print_etape("is eating", gettime() - philo->start,philo->id,&data->print);
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
	while (data->time_to_sleep)
	{
		philo_eat(arg);
		nbrofmeal++;
		if (nbrofmeal == data->what_enough)
			data->who_complt++;
		// pthread_mutex_lock(philo->eating);
	print_etape("is sleeping", gettime() - philo->start,philo->id,&data->print);
	sleep_well(data->time_to_sleep);
	print_etape("is thinking", gettime() - philo->start,philo->id,&data->print);
	// pthread_mutex_lock(&data->print);

	// 	printf("%ld %d is sleeping\n", gettime() - data->time_start, philo->id);
	// pthread_mutex_unlock(&data->print);
		// pthread_mutex_unlock(philo->eating);
	// pthread_mutex_lock(&data->print);
	// 	printf("%ld %d is thinking\n", gettime() - data->time_start, philo->id);
	// pthread_mutex_unlock(&data->print);
	}
	return (0);
}

int	check_death(t_data *arg)
{
	int	i;
	t_data	*data;
	data = (t_data *)arg;
				int j=0;
	i = 0;
	while (1)
	{
		i = 0;
		while (i < data->nbm_philos)
		{
			// printf("%d\n",pthread_mutex_lock(&data->print));
			pthread_mutex_lock(&data->print);
			if (gettime() - data->philos[i].last_meal > data->time_to_die)
			{
				printf("%ld %d died\n", gettime() - data->time_start, i);
				data->stop=1;
				while (j < data->nbm_philos)
				{
					pthread_mutex_destroy(&data->forks[j]);
					// pthread_create(&thread[i], NULL, philo_func, &data.philos[i]);
					j++;
				}
				// exit(0);
				// usleep(50);
				return (0);
			}
			pthread_mutex_unlock(&data->print);
			pthread_mutex_lock(&data->print);
			if (data->who_complt == data->nbm_philos)
			{
				printf("%ld  Done\n", gettime()
					- data->time_start);
				return (0);
			}
			pthread_mutex_unlock(&data->print);
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
	pthread_mutex_init(&data->print, NULL);
	i = 0;
	data->time_start = gettime();
	while (i < data->nbm_philos)
	{
		data->philos[i].data = data;
		data->philos[i].id = i;
		data->philos[i].start = data->time_start;
		// data->philos[i].eating = &data->eat[i];
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
	pthread_t	*thread;
	int			i;

	if (ac != 6 && ac != 5)
		return (0);
	i = 0;
	if (!take_args(ac, av, &data))
		return (0);
	if (!init_philos(&data))
		return (0);
	data.who_complt=0;
	data.stop=0;
	thread= malloc(sizeof(pthread_create)* data.nbm_philos);
	i = 0;
	while (i < data.nbm_philos)
	{
		pthread_create(&thread[i], NULL, philo_func, &data.philos[i]);
		i++;
	}
	i = 0;
	while (i < data.nbm_philos)
	{
		pthread_detach(thread[i]);
		// pthread_create(&thread[i], NULL, philo_func, &data.philos[i]);
		i++;
	}
	check_death(&data);
	i = 0;
	// while (i < data.nbm_philos)
	// {
	// 	pthread_mutex_destroy(&data.forks[i]);
	// 	// pthread_create(&thread[i], NULL, philo_func, &data.philos[i]);
	// 	i++;
	// }
	free(thread);
	// exit(0);
	// printf("dags\n");
	return 0;
}
