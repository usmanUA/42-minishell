
#include "minishell.h"

int	ft_push_env_var(t_shell *shell, char ***env_var)
{
	int	ind;

	ind = -1;
	while ((*env_var)[++ind])
	{
		if (!vec_push((*shell->input)->cmd, &(*env_var)[ind]))
		{
			if (env_var)
				free(*env_var);
			return (ft_free_prompt(shell, YES));
		}
	}
	free(*env_var);
	return (SUCCESS);
}