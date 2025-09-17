/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:51:41 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/17 12:25:58 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/* Lock two forks in correct order depending on philosopher ID */
static int	take_forks(t_philo *philo)
{
	t_params	*params;
	int			left;
	int			right;

	params = philo->params;
	left = philo->philo_id;
	right = (philo->philo_id + 1) % params->n;
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&params->forks[right]);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&params->forks[left]);
		print_state(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&params->forks[left]);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&params->forks[right]);
		print_state(philo, "has taken a fork");
	}
	return (1);
}

int	philo_eat(t_philo *philo)
{
	t_params	*params;

	params = philo->params;
	if (!take_forks(philo))
		return (0);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_timestamp_ms(params);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_state(philo, "is eating");
	smart_sleep(params->time_to_eat, philo);
	pthread_mutex_unlock(&params->forks[philo->philo_id]);
	pthread_mutex_unlock(&params->forks[(philo->philo_id + 1) % params->n]);
	return (1);
}

void	philo_sleep(t_philo *philo)
{
	print_state(philo, "is sleeping");
	smart_sleep(philo->params->time_to_sleep, philo);
}

void	philo_think(t_philo *philo)
{
	print_state(philo, "is thinking");
	if (philo->params->n % 2 != 0)
		usleep(500);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_params	*params;

	philo = (t_philo *)arg;
	params = philo->params;
	if (params->n % 2 == 0 && philo->philo_id % 2 == 0)
		usleep(params->time_to_eat * 1000 / 2);
	else if (params->n % 2 != 0 && philo->philo_id % 2 != 0)
		usleep(500);
	while (1)
	{
		if (params->stop)
			break ;
		if (!philo_eat(philo))
			break ;
		if (params->stop)
			break ;
		philo_sleep(philo);
		if (params->stop)
			break ;
		philo_think(philo);
	}
	return (NULL);
}
