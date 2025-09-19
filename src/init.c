/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:30:36 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/19 13:33:41 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/**
 * @brief Initialize simulation parameters from command-line arguments.
 *
 * Parses the number of philosophers, time to die, time to eat, time to sleep,
 * and optional number of times each philosopher must eat. Also initializes the
 * simulation stop flag and start time.
 *
 * @param params Pointer to the simulation parameters struct to initialize.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return 0 on success, non-zero if any argument is invalid.
 */
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
 * @brief Initialize all mutexes used in the simulation.
 *
 * Allocates and initializes mutexes for forks, printing, and stopping the
 * simulation. Each philosopher will use a fork mutex. Also initializes the
 * print_mutex for synchronized output and stop_mutex to control simulation
 * stopping.
 *
 * @param params Pointer to the simulation parameters.
 * @return 0 on success, non-zero if memory allocation or mutex init fails.
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
 * @brief Initialize philosopher structures and their meal mutexes.
 *
 * Allocates memory for all philosophers and initializes their fields:
 * philo_id, params pointer, last_meal_time, eat_count, and meal_mutex.
 *
 * @param philos Pointer to the array of philosopher structures.
 * @param params Pointer to the simulation parameters.
 * @return 0 on success, non-zero if memory allocation or mutex init fails.
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

/**
 * @brief Initialize the last meal time for all philosophers.
 *
 * This function sets the last_meal_time of each philosopher to the
 * current timestamp. Each philosopher's meal_mutex is locked while
 * updating to prevent race conditions.
 *
 * @param philos Array of philosopher structures.
 * @param params Pointer to the simulation parameters.
 */
void	init_last_meal_times(t_philo *philos, t_params *params)
{
	int	i;

	i = 0;
	while (i < params->n)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		philos[i].last_meal_time = get_timestamp_ms(params);
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
}

/**
 * @brief Start philosopher threads and handle thread creation errors.
 *
 * This function creates a thread for each philosopher. If a thread
 * cannot be created, it sets the stop flag, joins all previously
 * created threads, cleans up resources, and prints an error message.
 *
 * @param params Pointer to the simulation parameters.
 * @param philos Array of philosopher structures.
 *
 * @return 0 on success, or error code if thread creation failed.
 */
int	start_philos_threads(t_params *params, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < params->n)
	{
		if (pthread_create(&philos[i].thread, NULL,
				&philo_routine, &philos[i]) != 0)
		{
			pthread_mutex_lock(&params->stop_mutex);
			params->stop = 1;
			pthread_mutex_unlock(&params->stop_mutex);
			while (i > 0)
			{
				i--;
				pthread_join(philos[i].thread, NULL);
			}
			cleanup_resources(params, philos);
			return (print_error("Failed to create philosopher thread"));
		}
		i++;
	}
	return (0);
}
