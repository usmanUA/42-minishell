/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:47:48 by uahmed            #+#    #+#             */
/*   Updated: 2024/05/22 15:47:55 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

# define MINISHELL "\001\x1b[35m\002MiniWell😎:\001\x1b[0m\002"
# define VERSION "\001\x1b[34m\002V4.2\001\x1b[0m\002"
# define DOLLAR "\001\x1b[36m\002$ \001\x1b[0m\002"

# define BR		"\001\033[1;31m\002"
# define Y		"\001\033[0;33m\002"
# define T		"\001\033[0m\002"
# define V		"\001\033[35m\002"
# define G		"\001\033[32m\002"

enum e_button
{
	ON,
	OFF,
};

enum e_status_flags
{
	GREEN = 0,
	BROWN = 1,
	YELLOW = 126,
	RED = 127,
};

enum e_builtins
{
	EXTERNAL,
	CD,
	MY_ECHO,
	ENV,
	EXPORT,
	EXIT,
	PWD,
	UNSET,
};

enum e_process
{
	PARENT,
	CHILD,
	HEREDOC,
};

enum e_flags
{
	YES,
	NO,
	VALID,
	INVALID,
	NOFILE,
	FAILURE,
	SUCCESS,
};

enum e_consants
{
	COMMAND,
	REDIRECT,
	FD,
	FILENAME,
};

enum e_redir_types
{
	INPUT,
	OUTPUT,
};

#endif
