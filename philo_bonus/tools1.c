/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aechaoub <aechaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 20:12:25 by aechaoub          #+#    #+#             */
/*   Updated: 2023/01/24 20:14:29 by aechaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn_philo.h"

void	philo_eat(t_philo *philo, t_data *data, int nbrofmeal)
{
	sem_wait(data->mutex);
	sem_wait(data->mutex);
	printf("%ld %d has taken a fork\n", gettime() - data->time_start,
		philo->id);
	philo->last_meal = gettime();
	printf("%ld %d has taken a fork\n", gettime() - data->time_start,
		philo->id);
	printf("%ld %d is eating\n", gettime() - data->time_start, philo->id);
	sleep_well(data->time_to_eat);
	sem_post(data->mutex);
	sem_post(data->mutex);
	if (nbrofmeal == data->what_enough)
	{
		sem_post(data->all_eat);
	}
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nbm_philos);
	if (!data->philos)
		return (0);
	sem_unlink("/fork");
	sem_unlink("/death");
	sem_unlink("/all_eat");
	data->mutex = sem_open("/fork", O_CREAT | O_RDWR, 0644, data->nbm_philos);
	data->all_eat = sem_open("/death", O_CREAT | O_RDWR, 0644,
			data->nbm_philos);
	data->death = sem_open("/all_eat", O_CREAT | O_RDWR, 0644,
			data->nbm_philos);
	i = 0;
	while (i < data->nbm_philos)
	{
		data->philos[i].data = data;
		data->philos[i].id = i;
		data->philos[i].last_meal = gettime();
		data->philos[i].left_fork = data->mutex;
		data->philos[i].right_fork = data->mutex;
		i++;
	}
	data->time_start = gettime();
	return (1);
}
