/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aechaoub <aechaoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 18:33:48 by aechaoub          #+#    #+#             */
/*   Updated: 2023/01/28 18:45:30 by aechaoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_data *arg)
{
	int		i;
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->nbm_philos)
		{
			pthread_mutex_lock(&data->print);
			if (gettime() - data->philos[i].last_meal > data->time_to_die)
			{
				printf("%ld %d died\n", gettime() - data->time_start, i);
				return (0);
			}
			pthread_mutex_unlock(&data->print);
			pthread_mutex_lock(&data->print);
			if (data->who_complt == data->nbm_philos)
				return (0);
			pthread_mutex_unlock(&data->print);
			i++;
		}
		usleep(50);
	}
	return (1);
}
