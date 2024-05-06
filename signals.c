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

static void	ft_switch_echo(int button)
{
    struct  termios term;

    tcgetattr(STDIN_FILENO, &term);
    if (button == 0)
	term.c_lflag &= ~(ECHOCTL);
    else if (button == 1)
	term.c_lflag |= (ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static void	ft_ctrl_c_handler_exec(int num)
{
    write(1, "\n", 1);
}

static void	ft_ctrl_c_handler_main(int num)
{
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    (void)num;
}

void    ft_slash_signal(int place)
{
    if (place == 0)
        signal(SIGQUIT, SIG_IGN);
    else if (place == 1)

}

void	ft_parent_signals(int place, int button)
{
    struct  sigaction sa;

    ft_switch_echo(button);
    if (place == 0)
        sa.sa_handler = ft_ctrl_c_handler_main;
    else if (place == 1)
        sa.sa_handler = ft_ctrl_c_handler_exec;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    ft_quit_signal(place);
    if (place == 0)
}

