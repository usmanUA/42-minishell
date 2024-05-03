/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uahmed <uahmed@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 14:45:55 by uahmed            #+#    #+#             */
/*   Updated: 2024/04/30 14:45:56 by uahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "miniwell.h"

t_envp    *ft_listlast(t_envp *lst)
{
    while (lst != NULL)
    {
        if (lst->next == NULL)
            break ;
        lst = lst->next;
    }
    return (lst);
}

void    ft_listadd_back(t_envp **lst, t_envp *new)
{
    t_envp    *last_node;

    if (*lst)
    {
        last_node = ft_listlast(*lst);
        last_node->next = new;
    }
    else
    {
            *lst = new;
        }
    }
char    *string_n_copy(char *dest, char *source, int len)
{
    int    i;

    i = 0;
    while (i < len)
    {
        dest[i] = source[i];
        i++;
    }
    dest[i] = '\0';
    return (dest);
}

int    get_name_length(char *envp_string)
{
    int    i;

    i = 0;
    while (envp_string[i] && envp_string[i] != '=')
    {
        i++;
    }
    return (i);
}

void    print_envp_list_instance(t_shell *data)
{
    t_envp    *temp;

    temp = data->envp_linkedlist;
    while (temp && temp->env_name)
    {
        printf("%s", temp->env_name);
        printf("=");
        printf("%s\n", temp->env_value);
        temp = temp->next;
    }
}

t_envp *create_new_node(t_shell *data)
{
    t_envp    *node;

    node = (t_envp *)malloc(sizeof(t_envp));
    if (node == NULL)
    {
        //free_all
        //error msg???
        exit(1);
    }    
    node->env_name = NULL;
    node->env_value = NULL;
    node->next = NULL;
    return (node);
}

char    *get_name_of_envp_variable(t_shell *data, char *envp_string)
{
    char    *name;
    int        name_length;

    name_length = get_name_length(envp_string);
    name = (char *)malloc(sizeof(char) * (name_length + 1));
    if (name == NULL)
    {
        //free all
        exit (1); // NOTE: exit what?
    }
    name = string_n_copy(name, envp_string, name_length);
    return (name);
}

char    *get_value_of_envp_variable(t_shell *data, char *envp_string)
{
    char    *value;
    int        value_length;
    char    *value_string;

    value = ft_strchr(envp_string, '=');//what if no equals
    //if null error
    value++;
    value_length = ft_strlen(value);
    value_string = (char *)malloc(sizeof(char) * (value_length + 1));
    if (value_string == NULL)
    {
        //free all
        exit (1);
    }
    value_string = string_n_copy(value_string, value, value_length);
    return (value_string);
}

t_envp *add_envp_name_and_value(t_shell *data, t_envp *node, char *envp_string)
{
    node->env_name = get_name_of_envp_variable(data, envp_string);
    node->env_value = get_value_of_envp_variable(data, envp_string);
    return (node);
}

int    make_linked_list_of_envp(t_shell *data, char **envp)
{
    t_envp    *list;
    t_envp    *node;
    int        i;

    list = NULL;
    if (envp == NULL)// NOTE: If env is set to NULL (f.ex. by running "env -i") [this protects against SEGFAULT]
    {
        // TODO: error msg???
        return (0); // NOTE: Technically it's ok if envp is set to NULL -> so no error number
    }
    i = 0;
    while (envp[i])
    {
        node = create_new_node(data);
        //restructure - name and value here
        node = add_envp_name_and_value(data, node, envp[i]);
        ft_listadd_back(&list, node);
        i++;
    }
    data->envp_linkedlist = list;
    return (0);
}

