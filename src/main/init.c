/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:53:25 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:53:27 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "dongle.h"
#include "coder.h"
#include "monitor.h"

static int	start_coder_threads(t_sim *sim)
{
	int			i;
	t_coder_arg	*arg;

	i = 0;
	while (i < sim->num_coders)
	{
		arg = malloc(sizeof(t_coder_arg));
		if (!arg)
			return (0);
		arg->sim = sim;
		arg->id = i + 1;
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, arg) != 0)
		{
			free(arg);
			return (0);
		}
		sim->coders[i].started = 1;
		i++;
	}
	return (1);
}

static int	start_monitor_thread(t_sim *sim)
{
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
		return (0);
	sim->monitor_started = 1;
	return (1);
}

static void	stop_and_join_coders(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	i = 0;
	while (i < sim->num_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		sim->dongles[i].stop = 1;
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		if (sim->coders[i].started)
			pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	if (sim->monitor_started)
	{
		pthread_join(sim->monitor, NULL);
		sim->monitor_started = 0;
	}
}

int	init_all(t_sim *sim)
{
	init_simulation(sim);
	if (!init_dongles(sim) || !init_coders_array(sim))
	{
		cleanup(sim);
		return (0);
	}
	if (!start_coder_threads(sim))
	{
		stop_and_join_coders(sim);
		cleanup(sim);
		return (0);
	}
	if (!start_monitor_thread(sim))
	{
		stop_and_join_coders(sim);
		cleanup(sim);
		return (0);
	}
	return (1);
}
