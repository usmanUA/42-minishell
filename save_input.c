/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_user.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:55:03 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/16 13:55:05 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft/libft.h"
#include "miniwell.h"

void	ft_init_vars(t_vars *vars)
{
    // NOTE: Initialize vars struct
    vars->ind = -1;
    vars->end = 0;
    vars->len = 0;
    vars->fd = 0;
    vars->input_line = NULL;
}

static size_t	ft_user_ends(char *s)
{
    // NOTE: NOT used function
    while (*s)
    {
	if (!ft_isspace(*s))
	    return (0);
	s++;
    }
    return (1);
}

static void	ft_strings_end(t_vars *vars, int operator)
{
    // NOTE: finds the index for the end of the current string and update the var vars->end 
    // if there's operator then finds the index of next character followed by the operator
    // TODO: HANDLE QUOTATIONS as we should remove some of the QUOTATIONS based on the bash logic
    // WARN: THIS FUNCTION is IMPORTANT as there needs to do the above TODO.
    int ind;
    int	quote;

    quote = 0;
    ind = 0;
    if (operator == REDIRECT)
    {
	while (vars->input_line[vars->ind+ind]=='<' || vars->input_line[vars->ind+ind]=='>')
	    ind++;
    }
    else if (operator == FD)
    {
	while (ft_isdigit(vars->input_line[vars->ind+ind]))
	    ind++;
    }
    else if (operator == COMMAND)
    {
	if (vars->input_line[vars->ind] == '\'' || vars->input_line[vars->ind] == '\"')
	    quote = 1;
	while (vars->input_line[vars->ind + ind] && !ft_isspace(vars->input_line[vars->ind + ind]))
	{
	    while (quote && ft_isspace(vars->input_line[vars->ind+ind]))
	    {
		if (vars->input_line[vars->ind+ind] == '\'' || vars->input_line[vars->ind+ind] == '\"') 
		    quote = 1;
		++ind;
	    }
	    quote = 0;
	    ++ind;
	}
    }
    vars->end = vars->ind + ind; // NOTE: END updates here
}


static int ft_index_after_spaces(t_vars *vars)
{
    // NOTE: SKIPS all the spaces and moves the pointer to the non-space character 
    // if there's no non-space character after the space/spaces returns false
    int ind;
    int end;

    ind = 0;
    end = vars->end;
    if (ft_space_until_end(&vars->input_line[vars->end]))
	return (0);
    while (ft_isspace(vars->input_line[vars->end+ind]))
	ind++;
    vars->ind = vars->end + ind; // NOTE: index updates here
    return (1);
}

// static int ft_redirect_operator(char c)
// {
//     // NOTE: returns true if the current string is one of the redirect operators
// 	return (1);
//     if (!ft_strncmp(c, "<<", 2) || !ft_strncmp(c, ">>", 2))
// 	return (1);
//     return (0);
// }

void	ft_shift_pointer(t_vars *vars)
{
    // NOTE: moves the pointer either to the next | or null-terminator whatever comes first in the char *
    vars->ind = vars->end;
    while (vars->ind < vars->len)
    {
	if (vars->input_line[vars->ind] == '|') // NOTE: if encounters | stops.
	{
	    vars->ind++;
	    return ;
	}
	vars->ind++;
    }
}

char *ft_next_string(t_vars *vars, int op)
{
    // NOTE: returns the next string 
    char *s;

    if (!ft_index_after_spaces(vars)) // NOTE: skip spaces, vars->ind->string or 0 if all spaces 
	 return (NULL);
    ft_strings_end(vars, op); // NOTE: vars->end now points to the end of string
    s = ft_substr(vars->input_line, vars->ind, vars->end - vars->ind); // NOTE: malloc that string in heap and point str to it
    if (!s)
	return (NULL);
    return (s);
}

void	ft_next_pipe_null(t_vars *vars)
{

}

int ft_redirection(t_vars *vars)
{
    // NOTE: checks the operator first case
    int ind;

    ind = vars->ind;
    if (vars->input_line[ind] == '<' || vars->input_line[ind] == '>')
	return (1);
    while (ft_isdigit(vars->input_line[ind]))
	ind++;
    if (vars->input_line[ind] == '<' || vars->input_line[ind] == '>')
    {
	vars->fd = 1;
	return (1);
    }
    return (0);
}

int ft_save_input(t_vec *pipes, t_vars *vars)
{
    // NOTE: PARSE EVERYTHING
    t_input *input;
    char *str;

    ft_index_after_spaces(vars); 
    while (++vars->ind < vars->len) // NOTE: loop over the whole user input line
    {
	input = malloc(sizeof(t_input));
	if (!input)
	    return (0);
	if (ft_redirection(vars))
	{
	    if (!ft_operator_first(input, vars))
		continue ;
	}
	else 
	{
	    if (!ft_command_first(input, str, vars))
		continue ;
	}
	free(str);
	// Take pipe thing to operator and command thing
	// if (!ft_strncmp(str, "|", 1))
	// {
	//     if (ft_syntax_error(vars, vars->len - vars->end)) // NOTE: if there's syntax error after the |
	// 	ft_shift_pointer(vars); // NOTE: move to the end or to the next |
	//     vars->begin = 1;
	//     if (!vec_push(pipes, input)) // WARNING: pushing the input (input->redirect & input->cmd) here, is it valid?
	// 	return (0);
	//     free(str);
	//     str = ft_next_string(vars);
	//     if (!str)
	// 	break ;
	// }
    }
    return (1);
}
