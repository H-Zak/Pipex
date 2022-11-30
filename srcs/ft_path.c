/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:13:53 by zakariyaham       #+#    #+#             */
/*   Updated: 2022/11/30 17:57:13 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	*put_path(char **cmd_path, int i, char *c)
{
	char	*tmp;

	tmp = ft_strjoin(cmd_path[i], c);
	free(cmd_path[i]);
	return (tmp);
}

void	found_path(char **envp, t_vare *vare, int i)
{
	while (envp[i])
	{
		vare->path = ft_strnstr(envp[i], "PATH=", 5);
		if (vare->path != NULL)
			break ;
		free(vare->path);
		i++;
	}
}

char	*get_the_path(char **envp, char **cmd, t_vare *vare)
{
	int		i;

	i = 0;
	found_path(envp, vare, i);
	vare->path = ft_substr(vare->path, 5, (ft_strlen(vare->path) - 5));
	vare->cmd_path = ft_split(vare->path, ':');
	if (vare->cmd_path == NULL)
	{
		free_all(vare);
		return (NULL);
	}
	i = 0;
	while (vare->cmd_path[i])
	{
		vare->cmd_path[i] = put_path(vare->cmd_path, i, "/");
		vare->cmd_path[i] = put_path(vare->cmd_path, i, cmd[0]);
		if (access(vare->cmd_path[i], F_OK | X_OK) == 0)
			return (vare->cmd_path[i]);
		i++;
	}
	free_all(vare);
	return (NULL);
}

int	raccourci(char **argv, t_vare *vare, int i)
{
	int	j;

	j = 0;
	j = access(argv[i], F_OK | X_OK) != 0;
	if (argv[i][0] == '.' && argv[i][1] == '/' && j != 0)
	{
		write_error_2(argv[i], 3);
		return (2);
	}
	if (access(argv[i], F_OK | X_OK) == 0)
	{
		vare->the_path = ft_strdup(argv[i]);
		return (0);
	}
	return (1);
}
