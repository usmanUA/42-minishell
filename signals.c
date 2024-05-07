/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:45:59 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/06 15:46:01 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniwell.h"
#include <stdio.h>

int sigint_status;

static void	ft_switch_echo(int button)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	if (button == OFF)
		term.c_lflag &= ~(ECHOCTL);
	else if (button == ON)
		term.c_lflag |= (ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static void	ft_ctrl_c_handler_exec(int num)
{
	(void)num;
	write(1, "\n", 1);
}

static void	ft_ctrl_c_handler_main(int num)
{
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	sigint_status = BROWN; 
	(void)num;
}

void	ft_ctrl_slash_handler(int sig)
{
	ft_putendl_fd("Quit", 1);
	(void)sig;
}

void	ft_sigquit(int place)
{
	struct sigaction	sa;

	if (place == PARENT)
	{
		signal(SIGQUIT, SIG_IGN);
		return ;
	}
	else if (place == CHILD)
		sa.sa_handler = ft_ctrl_slash_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

void	ft_signals(int place, int button, int *status)
{
	struct sigaction	sa;

	sigint_status = *status;
	ft_switch_echo(button);
	if (place == PARENT)
		sa.sa_handler = ft_ctrl_c_handler_main;
	else if (place == CHILD)
		sa.sa_handler = ft_ctrl_c_handler_exec;
	sigemptyset(&sa.sa_mask);
	*status = sigint_status;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	ft_sigquit(place);
}
