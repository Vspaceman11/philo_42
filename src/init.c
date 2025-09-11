/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:30:36 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/11 14:48:32 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	init_params(t_params *params, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (print_error("Wrong number of arguments"));

	params->must_eat_count = -1;
	if (!ft_atol_positive(argv[1], &params->n))
		return (print_error("Invalid number of philosophers"));
	if (!ft_atol_positive(argv[2], &params->time_to_die))
		return (print_error("Invalid time to die"));
	if (!ft_atol_positive(argv[3], &params->time_to_eat))
		return (print_error("Invalid time to eat"));
	if (!ft_atol_positive(argv[4], &params->time_to_sleep))
		return (print_error("Invalid time to sleep"));
	if (argc == 6)
	{
		if (!ft_atol_positive(argv[5], &params->must_eat_count))
			return (print_error("Invalid number"
					" of times each philosopher must eat"));
	}

	params->stop = 0;
	gettimeofday(&params->start_time, NULL);
	return (0);
}

/**
 * @brief Initialize all mutexes in the params structure.
 *
 * @param params Pointer to simulation parameters.
 * @return 0 on success, 1 on error.
 */
int	init_mutexes(t_params *params)
{
	int	i;

	params->forks = malloc(sizeof(pthread_mutex_t) * params->n);
	if (!params->forks)
		return (print_error("Malloc failed for forks"));
	i = 0;
	while (i < params->n)
	{
		if (pthread_mutex_init(&params->forks[i], NULL) != 0)
			return (print_error("Mutex init failed for fork"));
		i++;
	}
	if (pthread_mutex_init(&params->print_mutex, NULL) != 0)
		return (print_error("Mutex init failed for print_mutex"));
	if (pthread_mutex_init(&params->stop_mutex, NULL) != 0)
		return (print_error("Mutex init failed for stop_mutex"));
	return (0);
}

/**
 * @brief Initialize philosophers array.
 *
 * @param philos Pointer to philosopher array (will be malloc'ed here).
 * @param params Pointer to simulation parameters.
 * @return 0 on success, 1 on error.
 */
int	init_philo(t_philo **philos, t_params *params)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * params->n);
	if (!*philos)
		return (print_error("Malloc failed for philosophers"));

	i = 0;
	while (i < params->n)
	{
		(*philos)[i].philo_id = i;
		(*philos)[i].params = params;
		(*philos)[i].last_meal_time = 0;
		(*philos)[i].eat_count = 0;
		if (pthread_mutex_init(&(*philos)[i].meal_mutex, NULL) != 0)
			return (print_error("Mutex init failed for meal_mutex"));
		i++;
	}
	return (0);
}
