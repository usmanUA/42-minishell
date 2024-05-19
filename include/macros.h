
#ifndef MACROS_H
#define MACROS_H


#define PROMPT	"\001\x1b[32mMiniWell\002\x1b[0m😎:\001\x1b[31mV0.1\002\x1b[0m$ "

typedef enum e_button
{
	ON,
	OFF,
}	t_button;

typedef enum	e_status_flags
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
