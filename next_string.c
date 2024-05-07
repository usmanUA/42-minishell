/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_string.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 09:40:12 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/24 09:40:14 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniwell.h"

char	*ft_expand_variable(t_vars *vars, t_envp *env_vars, int op);
int	ft_valid_char(char next, int check_digits);
int	ft_status_expansion(t_vars *vars, char c, int *ind);

static void	ft_unquoted_str_end(t_vars *vars, int *ind, char c)
{
	char	next;

	// NOTE: finds the index where the command ends
	// updates vars->qontinue based on the presence/absence of quotes right next to where command ends
	if (ft_status_expansion(vars, c, ind) == YES)	
		return;
	while (c != '\0' && c != '$')
	{
		if (vars->expand_it == YES && c != '?')
			break;
		if (c == '\'' || c == '\"')
		{
			vars->qontinue = YES;
			break ;
		}
		else if (ft_isspace(c) || c == '<' || c == '>')
			// NOTE: do we need to check for redirect operators?
			break ;
		c = vars->input_line[vars->ind + ++(*ind)];
	}
	next = vars->input_line[vars->ind + (*ind + 1)];
	if (c == '$' && ft_valid_char(next, YES) == VALID)
		vars->qontinue = YES;
}

void	ft_quoted_str_end(t_vars *vars, int quote, int *ind, int *dollar)
{
	char	c;
	char	next;

	c = vars->input_line[vars->ind];
	next = vars->input_line[vars->ind + 1];
	while (c && c != quote)
	{
		if (c == '$' && ft_valid_char(next, YES) == VALID && vars->s_quote == 0)
		{
			vars->qontinue = YES;
			*dollar = YES;
			break ;
		}
		(*ind)++;
		c = next;
		next = vars->input_line[vars->ind+(*ind+1)];
	}
	if (*dollar == NO)
	{
		if (!ft_isspace(vars->input_line[vars->ind+(*ind+1)]))
			vars->qontinue = YES;
		vars->s_quote = 0;
		vars->d_quote = 0;
		vars->increment = YES;
	}
}

static void	ft_commands_end(t_vars *vars, int quote, int *ind)
{
	char	c;
	char	next;
	int	dollar;

	// NOTE: knows the command string has started,
	// brings the pointer to the last char of the command string
	// takes care of the quotes if present
	dollar = NO;
	c = vars->input_line[vars->ind];
	next = vars->input_line[vars->ind + 1];
	if (c == '$' && ft_valid_char(next, YES) == VALID)
	{
		vars->expand_it = YES;
		c = next;
	}
	if (vars->d_quote || vars->s_quote)
	{
		if (vars->s_quote)
			quote = '\'';
		ft_quoted_str_end(vars, quote, ind, &dollar);
		return;
	}
	ft_unquoted_str_end(vars, ind, c);
}

static void	ft_strings_end(t_vars *vars, int operator)
{
	int	ind;

	// NOTE: finds the index for the end of the current string and update the var vars->end
	// if there's operator then finds the index of next character followed by the operator
	ind = 0;
	if (operator== REDIRECT)
	{
		while (vars->input_line[vars->ind + ind] == '<'
			|| vars->input_line[vars->ind + ind] == '>')
		{
			ind++;
			if (ind == 2)
				break ;
		}
	}
	else if (operator== FD)
	{
		while (ft_isdigit(vars->input_line[vars->ind + ind]))
			ind++;
	}
	else if (operator== COMMAND || operator== FILENAME)
		ft_commands_end(vars, '\"', &ind);
	vars->end = vars->ind + ind; // NOTE: END updates here
}

char	*ft_next_string(t_vars *vars, int op, t_envp *env_vars)
{
	char	*s;

	// NOTE: returns the next string
	vars->expand_it = NO;
	vars->expanded = NO;
	vars->qontinue = NO;
	vars->increment = NO;
	ft_strings_end(vars, op); // NOTE: vars->end now points to the end of string
	if (vars->expand_it == YES)
		s = ft_expand_variable(vars, env_vars, op);
	else
		s = ft_substr(vars->input_line, vars->ind, vars->end
				- vars->ind);                       
			// NOTE: malloc that string in heap and point str to it
	if ((vars->expand_it == NO && !s) || (vars->expand_it == YES
			&& vars->expanded == YES && !s)) // TODO: look back to the condition
		vars->malloc_flag = RED;
	return (s);
}
