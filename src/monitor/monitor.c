/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:55:28 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:55:30 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitor.h"

static int	handle_burnout(t_sim *sim, int idx)
{
	pthread_mutex_lock(&sim->stop_mutex);
	if (!sim->stop)
	{
		sim->stop = 1;
		print_log(sim, idx + 1, "burned out");
	}
	pthread_mutex_unlock(&sim->stop_mutex);
	signal_stop(sim);
	return (1);
}

static int	coder_burnout_check(t_sim *sim, int idx)
{
	long long	now;
	long long	deadline;
	int			comp;
	long long	last;
	int			is_comp;

	now = get_time_ms();
	pthread_mutex_lock(&sim->coders[idx].mutex);
	comp = sim->coders[idx].compiles_done;
	last = sim->coders[idx].last_compile_start;
	is_comp = sim->coders[idx].is_compiling;
	pthread_mutex_unlock(&sim->coders[idx].mutex);
	if (comp >= sim->compiles_required)
		return (0);
	if (comp == 0)
		deadline = sim->sim_start_abs + sim->time_to_burnout;
	else
		deadline = last + sim->time_to_burnout;
	if (!is_comp && now >= deadline)
		return (handle_burnout(sim, idx));
	return (0);
}

static int	check_burnout(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		if (coder_burnout_check(sim, i))
			return (1);
		i++;
	}
	return (0);
}

static void	check_completion_and_stop(t_sim *sim)
{
	if (all_coders_completed(sim))
	{
		pthread_mutex_lock(&sim->stop_mutex);
		sim->stop = 1;
		pthread_mutex_unlock(&sim->stop_mutex);
		signal_stop(sim);
	}
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (1)
	{
		pthread_mutex_lock(&sim->stop_mutex);
		if (sim->stop)
		{
			pthread_mutex_unlock(&sim->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->stop_mutex);
		if (check_burnout(sim))
			continue ;
		check_completion_and_stop(sim);
		usleep(1000);
	}
	return (NULL);
}
