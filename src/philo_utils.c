/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:43:49 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/10 15:54:01 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	print_state(t_philo *philo, const char *state)
{
	t_params	*params;

	params = philo->params;

	pthread_mutex_lock(&params->print_mutex);
	printf("%ld %d %s\n", get_timestamp_ms(params), philo->philo_id + 1, state);
	pthread_mutex_unlock(&params->print_mutex);
}

long	get_timestamp_ms(t_params *params)
{
	struct timeval	now;
	long			ms;

	gettimeofday(&now, NULL);
	ms = (now.tv_sec - params->start_time.tv_sec) * 1000
		+ (now.tv_usec - params->start_time.tv_usec) / 1000;
	return (ms);
}

void	*monitor_routine(void *arg)
{
	t_philo		*philos;
	t_params	*params;
	int			i;
	long		current_time;

	philos = (t_philo *)arg;
	params = philos[0].params;
	while (1)
	{
		i = 0;
		while (i < params->n)
		{
			current_time = get_timestamp_ms(params);
			pthread_mutex_lock(&philos[i].meal_mutex);
			if ((current_time - philos[i].last_meal_time) > params->time_to_die)
			{
				pthread_mutex_lock(&params->stop_mutex);
				params->stop = 1;
				pthread_mutex_unlock(&params->stop_mutex);
				print_state(&philos[i], "died");
				pthread_mutex_unlock(&philos[i].meal_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&philos[i].meal_mutex);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
