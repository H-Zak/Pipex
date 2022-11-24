/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:13:53 by zakariyaham       #+#    #+#             */
/*   Updated: 2022/11/24 18:09:18 by zhamdouc         ###   ########.fr       */
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

// void	free_path(char **cmd_path, char *path)
// {
// 	int	i;

// 	i = 0;
// 	if (path != NULL)
// 		free(path);
// 	if (cmd_path != NULL)
// 	{
// 		while (cmd_path[i])
// 		{
// 			free(cmd_path[i]);
// 			i++;
// 		}
// 		free(cmd_path);
// 	}
// }

char	*get_the_path(char **envp, char **cmd, t_vare *vare)
{
	int		i;
//free la vare
	i = 0;
	while (envp[i])
	{
		vare->path = ft_strnstr(envp[i], "PATH=", 5);
		if (vare->path != NULL)
			break;
		free(vare->path);
		i++;
	}
	vare->path = ft_substr(vare->path, 5, (ft_strlen(vare->path) - 5));
	vare->cmd_path = ft_split(vare->path, ':');//
	if (vare->cmd_path == NULL)
	{
		//free_path(vare->cmd_path, vare->path);
		free_all(vare);
		return (NULL);
	}
	i = 0;
	while (vare->cmd_path[i])
	{
		vare->cmd_path[i] = put_path(vare->cmd_path, i, "/");
		vare->cmd_path[i] = put_path(vare->cmd_path, i, cmd[0]);//si cmd[0] != ' '
		if (access(vare->cmd_path[i], F_OK | X_OK) == 0)
			return (vare->cmd_path[i]);//free dans le main
		i++;
	}
	//free_path(vare->cmd_path, vare->path);
	free_all(vare);
	return (NULL);
}
