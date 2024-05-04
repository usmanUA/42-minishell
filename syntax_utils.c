#include "miniwell.h"

static int ft_special_char(char s, int pipe_flag, int redir_flag)
{
    // NOTE: checks for prohibited special characters and returns 1 if true 
    if (pipe_flag != NOFLAG && redir_flag != NOFLAG)
    {
	if (s == '|')
	    return (YES);
	if (s == '<' || s == '>')
	    return (YES);
    }
    if (s == '(' || s == ')') 
	return (YES);
    if (s == '&' || s == ';' || s == '\\')
	return (YES);
    return (NO);
}

static int ft_redirect_operator(char *s, int *ind)
{
    if (!ft_strncmp(&s[*ind], "<<", 2) || !ft_strncmp(&s[*ind], ">>", 2))
    {
	*ind += 2;
	return (YES);
    }
    else if (!ft_strncmp(&s[*ind], "<", 1) || !ft_strncmp(&s[*ind], ">", 1))
    {
	++(*ind);
	return (YES);
    }
    return (NO);
}

static void ft_skip_spaces(char *s, int *ind)
{
    while (s[*ind] != '\0' && ft_isspace(s[*ind]))
	    ++(*ind);
}

int ft_prohibited_chars(t_vars *vars)
{
    // NOTE: loops over the user input and checks for prohibited special characters and returns 1 if true 
    int ind;
    
    ind = 0;
    ft_skip_spaces(vars->input_line, &ind);
    if (vars->input_line[ind] == '|')
	return (YES);
    while (ind < vars->len)
    {
	if (ft_redirect_operator(vars->input_line, &ind) == YES)
	{
	    ft_skip_spaces(vars->input_line, &ind);
	    if (ft_special_char(vars->input_line[ind], PIPE, REDIR) == YES)
		return (ft_token_error(vars->input_line[ind], 0));
	}
	else if (ft_special_char(vars->input_line[ind], NOFLAG, NOFLAG) == YES)
	    return (ft_token_error(vars->input_line[ind], 0));
	++ind;
    }
    return (NO);
}

