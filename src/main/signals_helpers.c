/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 10:09:22 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/22 10:09:24 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ft_switch_echo(int button)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	if (button == OFF)
		term.c_lflag &= ~(ECHOCTL);
	else if (button == ON)
		term.c_lflag |= (ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	ft_restore_terminal_settings(void)
{
	struct termios	term;

	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
