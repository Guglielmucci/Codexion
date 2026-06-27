/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fedegugl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:57:57 by fedegugl          #+#    #+#             */
/*   Updated: 2026/06/26 09:57:59 by fedegugl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include "../includes/codexion.h"

int			parse_args(int ac, char **av, t_sim *sim);
void		stop_and_cleanup(t_sim *sim);
void		cleanup(t_sim *sim);
int			init_all(t_sim *sim);
int			init_coders_array(t_sim *sim);
void		init_simulation(t_sim *sim);

#endif
