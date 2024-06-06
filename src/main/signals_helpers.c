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

	ft_memset(&term, 0, sizeof(struct termios));
	tcgetattr(STDIN_FILENO, &term);
	if (button == OFF)
		term.c_lflag &= ~(ECHOCTL);
	else if (button == ON)
		term.c_lflag |= (ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	ft_ctrl_c_handler_heredoc(int sig)
{
	if (sig == SIGINT)
		g_signal_status = BROWN;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}
