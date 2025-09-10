/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:51:41 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/10 15:54:41 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	philo_eat(t_philo *philo)
{
	t_params *params = philo->params;
	int first_fork = philo->philo_id;
	int second_fork = (philo->philo_id + 1) % params->n;

	pthread_mutex_lock(&params->forks[first_fork]);
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(&params->forks[second_fork]);
	print_state(philo, "has taken a fork");

	pthread_mutex_lock(&philo->meal_mutex);
	print_state(philo, "is eating");
	philo->last_meal_time = get_timestamp_ms(params);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_mutex);

	usleep(params->time_to_eat * 1000);

	pthread_mutex_unlock(&params->forks[second_fork]);
	pthread_mutex_unlock(&params->forks[first_fork]);
}

void	philo_sleep(t_philo *philo)
{
	print_state(philo, "is sleeping");
	usleep(philo->params->time_to_sleep * 1000);
}

void	philo_think(t_philo *philo)
{
	print_state(philo, "is thinking");
}

/**
 * @brief Thread routine for each philosopher.
 *
 * @param arg Pointer to t_philo struct.
 * @return NULL
 */
void	*philo_routine(void *arg)
{
	t_philo *philo;
	t_params *params;

	philo = (t_philo *)arg;
	params = philo->params;
	// Add delay for even numbered philosophers to prevent deadlock
	if (philo->philo_id % 2 == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&params->stop_mutex);
		if (params->stop)
		{
			pthread_mutex_unlock(&params->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&params->stop_mutex);
		philo_eat(philo);
		pthread_mutex_lock(&params->stop_mutex);
		if (params->stop)
		{
			pthread_mutex_unlock(&params->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&params->stop_mutex);
		philo_sleep(philo);
		pthread_mutex_lock(&params->stop_mutex);
		if (params->stop)
		{
			pthread_mutex_unlock(&params->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&params->stop_mutex);
		philo_think(philo);
	}
	return (NULL);
}

/**
 * @brief Start simulation by creating and joining philosopher threads.
 *
 * @param philos Array of philosophers.
 * @param params Simulation parameters.
 * @return 0 on success, 1 on error.
 */
int	start_simulation(t_philo *philos, t_params *params)
{
	int	i;

	i = 0;
	while (i < params->n)
	{
		if (pthread_create(&philos[i].thread, NULL,
				&philo_routine, &philos[i]) != 0)
			return (print_error("Failed to create thread"));
		i++;
	}
	i = 0;
	while (i < params->n)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (print_error("Failed to join thread"));
		i++;
	}
	return (0);
}

