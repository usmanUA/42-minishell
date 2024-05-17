
#include "minishell.h"

int	ft_status_expansion(t_vars *vars, char c, int *ind);
int	ft_valid_char(char next, int check_digits);

static	int	ft_quote_skipped(t_vars *vars, char quo)
{
	if (vars->input_line[++vars->ind] != quo)
		return (NO);
	++vars->ind;
	if (ft_isspace(vars->input_line[vars->ind]
			|| vars->input_line[vars->ind] == '\0'))
		vars->stop = 1;
	return (YES);
}

void	ft_skip_quotes(t_vars *vars)
{
	while (42)
	{
		if (vars->input_line[vars->ind] == '\"')
		{
			if (ft_quote_skipped(vars, '\"') == NO)
			{
				vars->d_quote = 1;
				return ;
			}
		}
		else if (vars->input_line[vars->ind] == '\'')
		{
			if (ft_quote_skipped(vars, '\'') == NO)
			{
				vars->s_quote = 1;
				return ;
			}
		}
		else
			break ;
	}
}

void	ft_unquoted_str_end(t_vars *vars, int *ind)
{
	char	next;
	char	c;

	// NOTE: finds the index where the command ends
	// updates vars->qontinue based on the presence/absence of quotes right next to where command ends
	c = vars->input_line[vars->ind + *ind];
	if (ft_status_expansion(vars, c, ind) == YES)	
		return;
	while (c != '\0' && c != '$' && c != '|')
	{
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
	if (c != '\0')
		next = vars->input_line[vars->ind + (*ind + 1)];
	if (c == '$' && ft_valid_char(next, YES) == VALID)
		vars->qontinue = YES;
}

static	void	ft_set_flags(t_vars *vars, char c, char next)
{
	if (!ft_isspace(next))// && c != '\"')
		vars->qontinue = YES;
	vars->s_quote = 0;
	vars->d_quote = 0;
	vars->increment = YES;
}

void	ft_quoted_str_end(t_vars *vars, int quote, int *ind, int *dollar)
{
	char	c;
	char	next;

	c = vars->input_line[vars->ind + *ind];
	next = vars->input_line[vars->ind + *ind + 1];
	if (ft_status_expansion(vars, c, ind) == YES)	
		return;
	while (c && c != quote)
	{
		if (c == '$' && ft_valid_char(next, YES) == VALID && vars->s_quote == 0)
		{
			vars->qontinue = YES;
			*dollar = YES;
			break ;
		}
		else if (vars->expand_it == YES && c != '\"' && c != '_' && !ft_isalnum(c))
		{
			vars->qontinue = YES;
			return;
		}
		(*ind)++;
		c = next;
		next = vars->input_line[vars->ind+(*ind+1)];
	}
	if (*dollar == NO)
		ft_set_flags(vars, c, next);
}

