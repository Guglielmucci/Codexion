/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:54:51 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:54:53 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	init_simulation(t_sim *sim)
{
	pthread_mutex_init(&sim->stop_mutex, NULL);
	pthread_mutex_init(&sim->print_mutex, NULL);
	sim->stop = 0;
	sim->monitor_started = 0;
	sim->sim_start_abs = get_time_ms();
}

int	init_coders_array(t_sim *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->num_coders);
	if (!sim->coders)
		return (0);
	i = 0;
	while (i < sim->num_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compiles_done = 0;
		sim->coders[i].last_compile_start = sim->sim_start_abs;
		sim->coders[i].is_compiling = 0;
		sim->coders[i].started = 0;
		pthread_mutex_init(&sim->coders[i].mutex, NULL);
		i++;
	}
	return (1);
}
