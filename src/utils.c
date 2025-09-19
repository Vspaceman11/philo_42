/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:22:29 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/19 13:33:10 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

#include <stdio.h>

/**
 * @brief Print an error message to stdout.
 *
 * Prints "Error: <msg>" and returns 1.
 *
 * @param msg Error message string.
 * @return Always returns 1.
 */
int	print_error(const char *msg)
{
	printf("Error: %s\n", msg);
	return (1);
}

/**
 * @brief Convert a string to a positive long integer.
 *
 * Only digits are allowed. Returns 1 on success and sets *out. Returns 0
 * if input is invalid or result is non-positive.
 *
 * @param str Input string to convert.
 * @param out Pointer to store the resulting long integer.
 * @return 1 if conversion succeeded, 0 otherwise.
 */
int	ft_atol_positive(const char *str, long *out)
{
	int		i;
	long	result;

	if (!str || !str[0])
		return (0);
	i = 0;
	result = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		result = result * 10 + (str[i] - '0');
		if (result < 0)
			return (0);
		i++;
	}
	if (result <= 0)
		return (0);
	*out = result;
	return (1);
}

/**
 * @brief Frees and destroys all allocated resources of the simulation.
 *
 * Destroys all mutexes for forks and philosophers, frees the philosophers
 * array and the forks array.
 *
 * @param params Pointer to the shared simulation parameters (t_params *).
 * @param philos Pointer to the array of philosophers (t_philo *).
 */
void	cleanup_resources(t_params *params, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < params->n)
	{
		pthread_mutex_destroy(&params->forks[i]);
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&params->print_mutex);
	pthread_mutex_destroy(&params->stop_mutex);
	free(philos);
	free(params->forks);
}
