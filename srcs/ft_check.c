/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:10:29 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/11/25 20:12:16 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	ft_check_open(char **argv, t_vare *vare, int argc)
{
	vare->the_path = NULL;
	vare->cmd1 = NULL;
	vare->cmd2 = NULL;
	vare->cmd_path = NULL;
	vare->path = NULL;
	vare->status = 0;
	vare->i = 0;
	if (argc != 5)
		return (ft_putstr_fd("The number of arguments is incorrect, try again\n", 2), 1);
	if (argv[2][0] == '\0' && argv[3][0] == '\0')
		return (ft_putstr_fd("bash: : command not found\nbash: : command not found\n", 2), 1);
	if (argv[2][0] == '\0' || argv[3][0] == '\0')
		return (ft_putstr_fd("bash: : command not found\n", 2), 1);
	vare->fd_in = open(argv[1], O_RDWR);
	if (vare->fd_in < 0)
		return (perror("open"), 1);
	vare->fd_out = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);//secu
	if (vare->fd_out < 0)
		return (perror("open"), 1);
	if (pipe(vare->pipe_fd) < 0 )
		return (perror("pipe"), 1);
	vare->cmd1 = ft_split(argv[2], ' ');//cmd[1] correspond a la command et le reste aux options, que faire si il commence par un " "
	if (vare->cmd1 == NULL)
		return (ft_putstr_fd("bash: : command not found\n", 2), 1);
	vare->cmd2 = ft_split(argv[3], ' ');//cmd[1] correspond a la command et le reste aux options, que faire si il commence par un " ", faut il une secu
	if (vare->cmd2 == NULL)
		return (ft_putstr_fd("bash: : command not found\n", 2), 1);
	return (0);
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
	return(0);
}

// int check_cmd(t_vare *vare)
// {
//     if(vare->cmd1[0])
// }