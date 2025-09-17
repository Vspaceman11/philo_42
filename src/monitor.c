/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 12:10:50 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/17 12:12:20 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"


static int	check_philo_death(t_philo *philo,
		t_params *params, long current_time)
{
	if ((philo->last_meal_time == 0 && current_time > params->time_to_die)
		|| (philo->last_meal_time > 0
			&& current_time - philo->last_meal_time > params->time_to_die))
	{
		print_state(philo, "died");
		pthread_mutex_lock(&params->stop_mutex);
		params->stop = 1;
		pthread_mutex_unlock(&params->stop_mutex);
		return (1);
	}
	return (0);
}

static void	init_all_ate_flag(t_params *params, int *all_ate_enough)
{
	if (params->must_eat_count > 0)
		*all_ate_enough = 1;
	else
		*all_ate_enough = 0;
}

static void	check_all_ate(t_philo *philos,
		t_params *params, int *all_ate_enough)
{
	int	i;

	i = 0;
	while (i < params->n)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		if (params->must_eat_count > 0
			&& philos[i].eat_count < params->must_eat_count)
			*all_ate_enough = 0;
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
	if (params->must_eat_count > 0 && *all_ate_enough)
	{
		pthread_mutex_lock(&params->stop_mutex);
		params->stop = 1;
		pthread_mutex_unlock(&params->stop_mutex);
	}
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
		init_all_ate_flag(params, &all_ate_enough);
		i = 0;
		while (i < params->n)
		{
			pthread_mutex_lock(&philos[i].meal_mutex);
			current_time = get_timestamp_ms(params);
			if (check_philo_death(&philos[i], params, current_time))
				return (pthread_mutex_unlock(&philos[i].meal_mutex), NULL);
			pthread_mutex_unlock(&philos[i].meal_mutex);
			i++;
		}
		check_all_ate(philos, params, &all_ate_enough);
		usleep(100);
	}
	return (NULL);
}
