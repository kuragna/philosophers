/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_mem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabourri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 14:04:58 by aabourri          #+#    #+#             */
/*   Updated: 2023/09/04 15:20:46 by aabourri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_MEM_H
# define PHILO_MEM_H

# include <stdlib.h>

typedef struct s_mem
{
	void	**ptr;
	size_t	lenght;
	size_t	capacity;
}	t_mem;

void	philo_push_mem(t_mem *mem, void *ptr);
void	philo_reset_mem(t_mem *mem);
int		philo_init_mem(t_mem *mem);

#endif // PHILO_MEM_H