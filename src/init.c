/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:30:36 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/09 18:03:33 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	init_params(t_params *params, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (print_error("Usage: ./philo n t_die t_eat t_sleep [must_eat]"));
	params->n = parse_positive_int(argv[1]);
	if (params->n <= 0)
		return (print_error("Number of philosophers must be greater than 0"));
	params->time_to_die = parse_positive_int(argv[2]);
	params->time_to_eat = parse_positive_int(argv[3]);
	params->time_to_sleep = parse_positive_int(argv[4]);
	if (params->time_to_die <= 0 || params->time_to_eat <= 0
		|| params->time_to_sleep <= 0)
		return (print_error("Time values must be positive integers"));
	params->must_eat_count = 0;
	if (argc == 6)
	{
		params->must_eat_count = parse_positive_int(argv[5]);
		if (params->must_eat_count <= 0)
			return (print_error("must_eat must be a positive integer"));
	}
	params->stop = 0;
	gettimeofday(&params->start_time, NULL);
	return (0);
}

/**
 * @brief Initialize forks (mutexes) for philosophers.
 *
 * @param params Pointer to simulation parameters.
 * @return 0 on success, 1 on error.
 */
int	init_forks(t_params *params)
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
	return (0);
}
