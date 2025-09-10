/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:51:41 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/10 14:50:52 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	philo_eat(t_philo *philo)
{
	t_params	*params;

	params = philo->params;
	pthread_mutex_lock(&params->forks[philo->philo_id]);
	pthread_mutex_lock(&params->forks[(philo->philo_id + 1) % params->n]);
	print_state(philo, "has taken a fork");
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_ms = get_timestamp_ms(params);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_state(philo, "is eating");
	usleep(params->time_to_eat * 1000);
	pthread_mutex_unlock(&params->forks[philo->philo_id]);
	pthread_mutex_unlock(&params->forks[(philo->philo_id + 1) % params->n]);
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
	t_philo		*philo;
	t_params	*params;

	philo = (t_philo *)arg;
	params = philo->params;

	while (!params->stop)
	{
		philo_eat(philo);
		philo_sleep(philo);
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

