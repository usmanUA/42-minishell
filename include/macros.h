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


typedef enum e_button
{
	ON,
	OFF,
}	t_button;

typedef enum e_status_flags
{
	GREEN = 0,
	BROWN = 1,
	YELLOW = 126,
	RED = 127,
}	t_status_flags;

typedef enum e_builtins
{
	EXTERNAL,
	CD,
	MY_ECHO,
	ENV,
	EXPORT,
	EXIT,
	PWD,
	UNSET,
}	t_builtins;

typedef enum e_process
{
	PARENT,
	CHILD,
	HEREDOC,
}	t_process;

typedef enum e_flags
{
	YES,
	NO,
	VALID,
	INVALID,
	NOFILE,
	FAILURE,
	SUCCESS,
}	t_flags;

typedef enum e_consants
{
	COMMAND,
	REDIRECT,
	FD,
	FILENAME,
}	t_constants;

typedef enum e_redir_types
{
	INPUT,
	OUTPUT,
}	t_redir_types;

#endif
