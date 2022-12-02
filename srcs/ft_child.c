/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_child.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 17:39:48 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/12/02 19:45:15 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	close_child1(t_vare *vare, int *pipe_fd);
int	close_child2(t_vare *vare, char **argv, int *pipe_fd);

int	child1(t_vare *vare, char **envp, int *pipe_fd, char **argv)
{
	int	n;

	vare->fd_in = open(argv[1], O_RDONLY);
	if (vare->fd_in < 0)
		return (write_error(argv[1], vare, "0011"), 1);
	if (close_child1(vare, pipe_fd) == 1)
		return (1);
	n = 0;
	n = check_access(argv, vare, 2);
	if (n == 1)
	{
		vare->the_path = get_the_path(envp, vare->cmd1, vare);
		if (vare->the_path == NULL)
			return (write_error_2(argv[2], 1, vare), 1);
	}
	if (n != 2 && execve(vare->the_path, vare->cmd1, envp) < 0)
		write_error("execve", vare, "0000");
	return (1);
}

int	close_child1(t_vare *vare, int *pipe_fd)
{
	if (dup2(vare->fd_in, STDIN_FILENO) < 0)
		return (write_error("dup2", vare, "1011"), 1);
	if (close(vare->fd_in) < 0)
		return (write_error("fd_in", vare, "0011"), 1);
	if (close(pipe_fd[0]) < 0)
		return (write_error("pipe_fd[0]", vare, "0010"), 1);
	if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		return (write_error("dup2", vare, "0010"), 1);
	if (close(pipe_fd[1]) < 0)
		return (write_error("pipe_fd[1]", vare, "0000"), 1);
	return (0);
}

int	child2(t_vare *vare, char **envp, int *pipe_fd, char **argv)
{
	int	n;

	n = 0;
	if (close_child2(vare, argv, pipe_fd) == 1)
		return (1);
	n = check_access(argv, vare, 3);
	if (n == 1)
	{
		vare->the_path = get_the_path(envp, vare->cmd2, vare);
		if (vare->the_path == NULL)
			return (write_error_2(argv[3], 1, vare), 1);
	}
	if (n != 2 && execve(vare->the_path, vare->cmd2, envp) < 0)
		write_error("execve", vare, "0000");
	return (1);
}

int	close_child2(t_vare *vare, char **argv, int *pipe_fd)
{
	vare->fd_out = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (vare->fd_out < 0)
		return (write_error(argv[4], vare, "0111"), 1);
	if (dup2(vare->fd_out, STDOUT_FILENO) < 0)
		return (write_error("dup2", vare, "0111"), 1);
	if (close(vare->fd_out) < 0)
		return (write_error("fd_out", vare, "0011"), 1);
	if (close(pipe_fd[1]) < 0)
	{
		ft_putstr_fd("hello\n", 2);
		return (write_error("pipe_fd[1]", vare, "0001"), 1);
	}
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		return (write_error("dup2", vare, "0010"), 1);
	if (close(pipe_fd[0]) < 0)
		return (write_error("pipe[0]", vare, "0000"), 1);
	return (0);
}
