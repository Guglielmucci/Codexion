/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:49:51 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:49:54 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

static void	interruptible_sleep(t_sim *sim, int ms)
{
	int	elapsed;

	elapsed = 0;
	while (elapsed < ms && is_simulation_running(sim))
	{
		ms_sleep(1);
		elapsed++;
	}
}

static int	do_compile(t_sim *sim, int idx)
{
	int	running;

	pthread_mutex_lock(&sim->coders[idx].mutex);
	sim->coders[idx].is_compiling = 1;
	sim->coders[idx].last_compile_start = get_time_ms();
	pthread_mutex_unlock(&sim->coders[idx].mutex);
	print_log(sim, idx + 1, "is compiling");
	interruptible_sleep(sim, sim->time_to_compile);
	running = is_simulation_running(sim);
	pthread_mutex_lock(&sim->coders[idx].mutex);
	sim->coders[idx].is_compiling = 0;
	if (running)
		sim->coders[idx].compiles_done++;
	pthread_mutex_unlock(&sim->coders[idx].mutex);
	return (running);
}

static int	try_compile(t_sim *sim, int idx, int first, int second)
{
	long long	deadline;
	int			compiled;

	pthread_mutex_lock(&sim->coders[idx].mutex);
	deadline = sim->coders[idx].last_compile_start + sim->time_to_burnout;
	pthread_mutex_unlock(&sim->coders[idx].mutex);
	if (take_two_dongles(sim, idx, deadline) == -1)
		return (0);
	compiled = do_compile(sim, idx);
	release_dongles_for_coder(sim, first, second);
	return (compiled);
}

static int	coder_compile_cycle(t_sim *sim, int idx)
{
	int	first;
	int	second;
	int	comp_done;

	get_dongles(sim, idx, &first, &second);
	pthread_mutex_lock(&sim->coders[idx].mutex);
	comp_done = sim->coders[idx].compiles_done;
	pthread_mutex_unlock(&sim->coders[idx].mutex);
	if (comp_done >= sim->compiles_required)
		return (0);
	if (!try_compile(sim, idx, first, second))
	{
		return (0);
	}
	return (1);
}

void	*coder_routine(void *arg)
{
	t_sim	*sim;
	int		id;
	int		idx;

	sim = ((t_coder_arg *)arg)->sim;
	id = ((t_coder_arg *)arg)->id;
	free(arg);
	idx = id - 1;
	while (is_simulation_running(sim))
	{
		if (!coder_compile_cycle(sim, idx))
			break ;
		print_log(sim, id, "is debugging");
		interruptible_sleep(sim, sim->time_to_debug);
		if (!is_simulation_running(sim))
			break ;
		print_log(sim, id, "is refactoring");
		interruptible_sleep(sim, sim->time_to_refactor);
	}
	return (NULL);
}
