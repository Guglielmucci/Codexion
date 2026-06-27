/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:53:46 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:53:48 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	main(int ac, char **av)
{
	t_sim	sim;

	memset(&sim, 0, sizeof(sim));
	if (!parse_args(ac, av, &sim))
		return (1);
	if (!init_all(&sim))
		return (1);
	stop_and_cleanup(&sim);
	return (0);
}
