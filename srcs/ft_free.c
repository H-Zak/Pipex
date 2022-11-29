/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:06:16 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/11/29 17:54:10 by zhamdouc         ###   ########.fr       */
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
		tab = NULL;
	}
}

void	free_all(t_vare *vare)
{
	int	i;

	i = 0;
	if (vare->the_path != NULL)
	{
		free(vare->the_path);
		vare->the_path = NULL;
	}
	if (vare->path != NULL)
	{
		free(vare->path);
		vare->path = NULL;
	}
	free_tab(vare->cmd1, 0);
	free_tab(vare->cmd2, 0);
	free_tab(vare->cmd_path, 0);
}

int	end_close(t_vare *vare)
{
	if (close(vare->pipe_fd[0]) < 0)
		return (write_error("pipe_fd[0]", vare, "1111"), 1);
	if (close(vare->pipe_fd[1]) < 0)
		return (write_error("pipe_fd[1]", vare, "1111"), 1);
	if (close(vare->fd_in) < 0)
		return (write_error("fd_in", vare, "1111"), 1);
	if (close(vare->fd_out) < 0)
		return (write_error("fd_out", vare, "1111"), 1);
	free_all(vare);
	return (0);
}
