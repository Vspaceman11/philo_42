/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:43:49 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/11 17:40:13 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	print_state(t_philo *philo, const char *state)
{
	t_params	*params;

	params = philo->params;

	pthread_mutex_lock(&params->stop_mutex);
	if (params->stop)
	{
		pthread_mutex_unlock(&params->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&params->stop_mutex);

	pthread_mutex_lock(&params->print_mutex);
	pthread_mutex_lock(&params->stop_mutex);
	if (!params->stop)
		printf("%ld %d %s\n", get_timestamp_ms(params),
			philo->philo_id + 1, state);
	pthread_mutex_unlock(&params->stop_mutex);
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
	int			all_ate_enough;

	philos = (t_philo *)arg;
	params = philos[0].params;
	while (1)
	{
		i = 0;
		all_ate_enough = (params->must_eat_count <= 0) ? 0 : 1;
		while (i < params->n)
		{
			current_time = get_timestamp_ms(params);
			pthread_mutex_lock(&philos[i].meal_mutex);
			if ((philos[i].last_meal_time == 0
					&& current_time > params->time_to_die)
				|| (philos[i].last_meal_time > 0
					&& (current_time - philos[i].last_meal_time)
					> params->time_to_die))
			{
				print_state(&philos[i], "died");
				pthread_mutex_lock(&params->stop_mutex);
				params->stop = 1;
				pthread_mutex_unlock(&params->stop_mutex);
				pthread_mutex_unlock(&philos[i].meal_mutex);
				return (NULL);
			}
			if (params->must_eat_count > 0
				&& philos[i].eat_count < params->must_eat_count)
				all_ate_enough = 0;
			pthread_mutex_unlock(&philos[i].meal_mutex);
			i++;
		}
		if (params->must_eat_count > 0 && all_ate_enough)
		{
			pthread_mutex_lock(&params->stop_mutex);
			params->stop = 1;
			pthread_mutex_unlock(&params->stop_mutex);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
