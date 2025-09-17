/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:51:41 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/17 09:38:45 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	philo_eat(t_philo *philo)
{
	t_params	*params;
	int			left_fork;
	int			right_fork;

	params = philo->params;
	left_fork = philo->philo_id;
	right_fork = (philo->philo_id + 1) % params->n;
	// if (philo->philo_id % 2 == 0)
	// 	usleep(1000);
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&params->forks[right_fork]);
		pthread_mutex_lock(&params->stop_mutex);
		if (params->stop)
		{
			pthread_mutex_unlock(&params->stop_mutex);
			pthread_mutex_unlock(&params->forks[right_fork]);
			return (0);
		}
		pthread_mutex_unlock(&params->stop_mutex);
		print_state(philo, "has taken a fork");
		if (params->n == 1)
		{
			pthread_mutex_unlock(&params->forks[right_fork]);
			return (0);
		}
		pthread_mutex_lock(&params->forks[left_fork]);
		pthread_mutex_lock(&params->stop_mutex);
		if (params->stop)
		{
			pthread_mutex_unlock(&params->stop_mutex);
			pthread_mutex_unlock(&params->forks[left_fork]);
			pthread_mutex_unlock(&params->forks[right_fork]);
			return (0);
		}
		pthread_mutex_unlock(&params->stop_mutex);
		print_state(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&params->forks[left_fork]);
		pthread_mutex_lock(&params->stop_mutex);
		if (params->stop)
		{
			pthread_mutex_unlock(&params->stop_mutex);
			pthread_mutex_unlock(&params->forks[left_fork]);
			return (0);
		}
		pthread_mutex_unlock(&params->stop_mutex);
		print_state(philo, "has taken a fork");
		if (params->n == 1)
		{
			pthread_mutex_unlock(&params->forks[left_fork]);
			return (0);
		}
		pthread_mutex_lock(&params->forks[right_fork]);
		pthread_mutex_lock(&params->stop_mutex);
		if (params->stop)
		{
			pthread_mutex_unlock(&params->stop_mutex);
			pthread_mutex_unlock(&params->forks[right_fork]);
			pthread_mutex_unlock(&params->forks[left_fork]);
			return (0);
		}
		pthread_mutex_unlock(&params->stop_mutex);
		print_state(philo, "has taken a fork");
	}
	pthread_mutex_lock(&params->stop_mutex);
	if (params->stop)
	{
		pthread_mutex_unlock(&params->stop_mutex);
		pthread_mutex_unlock(&params->forks[left_fork]);
		pthread_mutex_unlock(&params->forks[right_fork]);
		return (0);
	}
	pthread_mutex_unlock(&params->stop_mutex);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_timestamp_ms(params);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_lock(&params->stop_mutex);
	if (params->stop)
	{
		pthread_mutex_unlock(&params->stop_mutex);
		pthread_mutex_unlock(&params->forks[left_fork]);
		pthread_mutex_unlock(&params->forks[right_fork]);
		return (0);
	}
	pthread_mutex_unlock(&params->stop_mutex);
	print_state(philo, "is eating");
	smart_sleep(params->time_to_eat, philo);
	pthread_mutex_lock(&params->stop_mutex);
	if (params->stop)
	{
		pthread_mutex_unlock(&params->stop_mutex);
		pthread_mutex_unlock(&params->forks[left_fork]);
		pthread_mutex_unlock(&params->forks[right_fork]);
		return (0);
	}
	pthread_mutex_unlock(&params->stop_mutex);
	pthread_mutex_unlock(&params->forks[left_fork]);
	pthread_mutex_unlock(&params->forks[right_fork]);
	return (1);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->stop_mutex);
	if (philo->params->stop)
	{
		pthread_mutex_unlock(&philo->params->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->params->stop_mutex);
	print_state(philo, "is sleeping");
	smart_sleep(philo->params->time_to_sleep, philo);
	pthread_mutex_lock(&philo->params->stop_mutex);
	if (philo->params->stop)
	{
		pthread_mutex_unlock(&philo->params->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->params->stop_mutex);
}

void	philo_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->stop_mutex);
	if (philo->params->stop)
	{
		pthread_mutex_unlock(&philo->params->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->params->stop_mutex);
	print_state(philo, "is thinking");
	pthread_mutex_lock(&philo->params->stop_mutex);
	if (philo->params->stop)
	{
		pthread_mutex_unlock(&philo->params->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->params->stop_mutex);
}

/**
 * @brief Thread routine for each philosopher.
 *
 * @param arg Pointer to t_philo struct.
 * @return NULL
 */
void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_params	*params;

	philo = (t_philo *)arg;
	params = philo->params;
	if (philo->philo_id % 2 == 0)
		usleep(params->time_to_eat / 2);
	else if (philo->philo_id % 2 != 0)
		usleep(philo->philo_id * 300);
	while (1)
	{
		pthread_mutex_lock(&params->stop_mutex);
		if (params->stop)
		{
			pthread_mutex_unlock(&params->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&params->stop_mutex);
		if (philo_eat(philo))
		{
			pthread_mutex_lock(&params->stop_mutex);
			if (params->stop)
			{
				pthread_mutex_unlock(&params->stop_mutex);
				break ;
			}
			pthread_mutex_unlock(&params->stop_mutex);

			philo_sleep(philo);
			philo_think(philo);
		}
		else
			break ;
	}
	return (NULL);
}
