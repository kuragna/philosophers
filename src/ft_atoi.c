/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 18:50:47 by aabourri          #+#    #+#             */
/*   Updated: 2023/07/16 16:43:16 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	ft_isspace(int c)
{
	if (c == ' ' || c == '\f' || c == '\n'
		|| c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);	
}

int	ft_atoi(const char *str)
{
	int	re;
	int	i;
	int	sign;

	if (!str)
		return (0);
	i = 0;
	sign = 1;
	re = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' && str[i + 1] != '+')
	{
		i++;
		sign = -1;
	}
	if (str[i] == '+')
		i++;
	while ((48 <= str[i] && 57 >= str[i]))
	{
		re = re * 10 + (int)(str[i] - 48);
		i++;
	}
	return (re * sign);
}
