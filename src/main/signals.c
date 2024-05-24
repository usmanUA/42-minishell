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
#include "minishell.h"

void	ft_switch_echo(int button);

static void	ft_ctrl_c_handler_exec(int sig)
{
	if (sig == SIGINT)
		g_signal_status = SIGINT + 128;
	write(1, "\n", 1);
}

static void	ft_ctrl_c_handler_main(int sig)
{
	if (sig == SIGINT)
		g_signal_status = BROWN;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_ctrl_slash_handler(int sig)
{
	if (sig == SIGQUIT)
		g_signal_status = SIGQUIT + 128;
	ft_putendl_fd("Quit", 1);
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

	ft_switch_echo(button);
	if (place == PARENT)
		sa.sa_handler = ft_ctrl_c_handler_main;
	else if (place == CHILD)
		sa.sa_handler = ft_ctrl_c_handler_exec;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	ft_sigquit(place);
}
