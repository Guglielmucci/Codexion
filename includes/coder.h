/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:57:21 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:57:22 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODER_H
# define CODER_H

# include "../includes/codexion.h"

int			take_two_dongles(t_sim *sim, int idx, long long deadline);
void		*coder_routine(void *arg);
int			is_simulation_running(t_sim *sim);
void		get_dongles(t_sim *sim, int idx, int *first, int *second);
void		release_dongles_for_coder(t_sim *sim, int first, int second);
void		release_dongle(t_sim *sim, int dongle_id);

#endif
