/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:06:16 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/11/28 15:10:24 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_tab(char **tab, int i)
{
	if (tab != NULL)
	{
		while (tab[i])
		{
			free(tab[i]);
			i++;
		}
		free(tab);
	}
}

void	free_all(t_vare *vare)
{
	int	i;

	i = 0;
	if (vare->the_path != NULL)
		free(vare->the_path);
	if (vare->path != NULL)
		free(vare->path);
	free_tab(vare->cmd1, 0);
	free_tab(vare->cmd2, 0);
	free_tab(vare->cmd_path, 0);
}

int	end_close(t_vare *vare)
{
	if (close(vare->pipe_fd[0]) < 0)
		return (perror("close"), 1);
	if (close(vare->pipe_fd[1]) < 0)
		return (perror("close"), 1);
	if (close(vare->fd_in) < 0)
		return (perror("close"), 1);
	if (close(vare->fd_out) < 0)
		return (perror("close"), 1);
	free_all(vare);
	return (0);
}
