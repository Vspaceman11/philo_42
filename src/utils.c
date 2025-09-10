/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:22:29 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/10 15:52:20 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

#include <stdio.h>

/**
 * @brief Prints an error message to stderr.
 *
 * @param msg The error message to display.
 * @return Always returns 1 (convention for error in init functions).
 */
int	print_error(const char *msg)
{
	printf("Error: %s\n", msg);
	return (1);
}

/**
 * @brief Convert string to positive long.
 *
 * Accepts only digits (no signs, no spaces).
 * Returns 1 on success, 0 on error.
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
