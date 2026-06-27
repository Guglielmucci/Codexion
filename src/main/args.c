/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:53:00 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:53:02 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	get_scheduler_type(const char *str, t_scheduler *out)
{
	if (strcmp(str, "fifo") == 0)
	{
		*out = SCHEDULER_FIFO;
		return (1);
	}
	if (strcmp(str, "edf") == 0)
	{
		*out = SCHEDULER_EDF;
		return (1);
	}
	return (0);
}

static int	parse_int(const char *s, int *out)
{
	int	i;

	i = 0;
	if (!s || s[0] == '\0')
		return (0);
	if (s[0] == '-')
	{
		if (s[1] == '\0')
			return (0);
		i = 1;
	}
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	*out = atoi(s);
	return (1);
}

static int	are_args_valid(t_sim *sim)
{
	if (sim->num_coders <= 0)
		return (0);
	if (sim->time_to_burnout <= 0 || sim->dongle_cooldown < 0)
		return (0);
	if (sim->time_to_compile <= 0 || sim->time_to_debug <= 0)
		return (0);
	if (sim->time_to_refactor <= 0 || sim->compiles_required <= 0)
		return (0);
	return (1);
}

int	parse_args(int ac, char **av, t_sim *sim)
{
	if (ac != 9)
		return (0);
	if (!parse_int(av[1], &sim->num_coders)
		|| !parse_int(av[2], &sim->time_to_burnout)
		|| !parse_int(av[3], &sim->time_to_compile)
		|| !parse_int(av[4], &sim->time_to_debug)
		|| !parse_int(av[5], &sim->time_to_refactor)
		|| !parse_int(av[6], &sim->compiles_required)
		|| !parse_int(av[7], &sim->dongle_cooldown))
		return (0);
	if (!get_scheduler_type(av[8], &sim->scheduler))
		return (0);
	if (!are_args_valid(sim))
		return (0);
	return (1);
}
