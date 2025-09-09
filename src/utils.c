/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:22:29 by vpushkar          #+#    #+#             */
/*   Updated: 2025/09/09 17:32:54 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

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
