/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:43:49 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/17 11:08:27 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"


void	print_state(t_philo *philo, const char *state)
{
	t_params	*params;

	params = philo->params;
	pthread_mutex_lock(&params->print_mutex);
	if (!params->stop)
		printf("%ld %d %s\n", get_timestamp_ms(params),
			philo->philo_id + 1, state);
	pthread_mutex_unlock(&params->print_mutex);
}

long	get_timestamp_ms(t_params *params)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - params->start_time.tv_sec) * 1000
		+ (now.tv_usec - params->start_time.tv_usec) / 1000);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	is_dead(t_philo *philo)
{
	int	dead;

	pthread_mutex_lock(&philo->params->stop_mutex);
	dead = philo->params->stop;
	pthread_mutex_unlock(&philo->params->stop_mutex);
	return (dead);
}

void	smart_sleep(long ms, t_philo *philo)
{
	long	start;

	start = get_time();
	while (!is_dead(philo) && get_time() - start < ms)
		usleep(500);
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
		if (params->must_eat_count > 0)
			all_ate_enough = 1;
		else
			all_ate_enough = 0;
		i = 0;
		while (i < params->n)
		{
			pthread_mutex_lock(&philos[i].meal_mutex);
			current_time = get_timestamp_ms(params);
			if ((philos[i].last_meal_time == 0
					&& current_time > params->time_to_die)
				|| (philos[i].last_meal_time > 0
					&& current_time - philos[i].last_meal_time > params->time_to_die))
			{
				print_state(&philos[i], "died");
				pthread_mutex_lock(&params->stop_mutex);
				params->stop = 1;
				pthread_mutex_unlock(&params->stop_mutex);
				pthread_mutex_unlock(&philos[i].meal_mutex);
				return (NULL);
			}
			if (params->must_eat_count > 0 && philos[i].eat_count < params->must_eat_count)
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
		usleep(100);
	}
	return (NULL);
}
