/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zakariyahamdouchi <zakariyahamdouchi@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:13:53 by zakariyaham       #+#    #+#             */
/*   Updated: 2022/11/23 17:23:59 by zakariyaham      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	*put_path(char **cmd_path, int i, char *c)
{
	char *tmp;

	tmp = ft_strjoin(cmd_path[i], c);
	free(cmd_path[i]);
	return (tmp);

}

void	free_all(char **cmd_path, char *path)
{
	int	i;

	i = 0;
	if (path != NULL)
		free(path);
	if (cmd_path != NULL)
	{
		while (cmd_path[i])
		{
			free(cmd_path[i]);
			i++;
		}
		free(cmd_path);
	}
}

char	*get_the_path(char **envp, char **cmd, t_list list)
{
	//char	**cmd_path;
	//char	*path;
	int		i;
//free la list
	i = 0;
	while (envp[i])
	{
		list.path = ft_strnstr(envp[i], "PATH=", 5);
		if (list.path != NULL)
			break;
		free(list.path);
		i++;
	}
	list.path = ft_substr(path, 5, (ft_strlen(path) - 5));
	list.cmd_path = ft_split(path, ':');//
	if (list.cmd_path == NULL)
	{
		free_all(list.cmd_path, list.path);
		return (NULL);
	}
	i = 0;
	while (list.cmd_path[i])
	{
		list.cmd_path[i] = put_path(list.cmd_path, i, "/");
		list.cmd_path[i] = put_path(list.cmd_path, i, cmd[0]);//si cmd[0] != ' '
		if (access(cmd_path[i], F_OK | X_OK) == 0)
			return (cmd_path[i]);//free dans le main
		i++;
	}
	free_all(cmd_path, path);
	return (NULL);
}
