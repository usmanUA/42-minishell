#include "miniwell.h"
#include <stdio.h>
#include <unistd.h>

void	ft_print_vecs(t_vec *pipes)
{
	int		ind;
	int		i;
	int		j;
	t_input	*input;
	t_vec	*cmd;
	t_vec	*new_fds;
	t_vec	*orig_fds;
	int		fd;

	ind = -1;
	printf("pipes: len: %zu, alloc_mem: %zu, size: %zu\n", pipes->len,
			pipes->mem_alloc, pipes->size);
	while (++ind < (int)pipes->len)
	{
		input = *(t_input **)&pipes->mem[ind * pipes->size];
		cmd = input->cmd;
		new_fds = input->new_fds;
		orig_fds = input->orig_fds;
		printf("cmd: len: %zu, alloc_mem: %zu, size: %zu\n", cmd->len,
				cmd->mem_alloc, cmd->size);
		if (new_fds)
		{
			printf("new_fds: len: %zu, alloc_mem: %zu, size: %zu\n",
					new_fds->len, new_fds->mem_alloc, new_fds->size);
			j = -1;
			while (++j < (int)new_fds->len)
			{
				fd = *(int *)vec_get(new_fds, j);
				printf("new fd: %d\n", fd);
				//		close(fd);
			}
		}
		if (orig_fds)
		{
			printf("orig_fds: len: %zu, alloc_mem: %zu, size: %zu\n",
					orig_fds->len, orig_fds->mem_alloc, orig_fds->size);
			j = -1;
			while (++j < (int)new_fds->len)
				printf("new fd: %d\n", *(int *)vec_get(orig_fds, j));
		}
		i = -1;
		while (++i < (int)cmd->len)
			printf("cmd and args: %s\n", *(char **)vec_get(cmd, i));
		printf("file_flag: %d\n", *input->file_flag);
	}
}
