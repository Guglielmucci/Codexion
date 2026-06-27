/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:53:13 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:53:16 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "dongle.h"

static void	join_all_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		if (sim->coders[i].started)
			pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	if (sim->monitor_started)
		pthread_join(sim->monitor, NULL);
}

static void	free_dongles(t_sim *sim)
{
	int	i;

	if (!sim->dongles)
		return ;
	i = 0;
	while (i < sim->num_coders)
	{
		while (sim->dongles[i].heap_size > 0)
			free(heap_pop(&sim->dongles[i]));
		free(sim->dongles[i].heap);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
	free(sim->dongles);
}

void	cleanup(t_sim *sim)
{
	int	i;

	if (sim->coders)
	{
		i = 0;
		while (i < sim->num_coders)
		{
			pthread_mutex_destroy(&sim->coders[i].mutex);
			i++;
		}
		free(sim->coders);
	}
	free_dongles(sim);
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->print_mutex);
}

void	stop_and_cleanup(t_sim *sim)
{
	join_all_threads(sim);
	cleanup(sim);
}
