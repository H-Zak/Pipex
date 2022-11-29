/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 19:16:08 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/11/29 19:15:02 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int		child1(t_vare *vare, char **envp, int *pipe_fd, char **argv);
int		child2(t_vare *vare, char **envp, int *pipe_fd, char **argv);
void	free_all(t_vare *vare);
int		loop(t_vare *vare, char **envp, char **argv, pid_t pid);

//on doit pouvoir lire le premier fichier qu'on nous envoie, il faut verifier qu'on a le droit de lire dessus sinon envoyer une erreur et faire la deuxieme commande si possible
int	main(int argc, char **argv, char **envp)
{
	t_vare	vare;
	pid_t	pid[2];
	int		n;

	n = 0;
	if (ft_check_open(argv, &vare, argc) != 0)
		return (free_all(&vare), 1);
	while (vare.i < 2)
	{
		pid[vare.i] = fork();
		if (pid[vare.i] < 0)
			return (write_error("fork_pid", &vare, "1111"), 1);
		if (loop(&vare, envp, argv, pid[vare.i]) != 0)
			return (1);
		vare.i++;
	}
	close(vare.pipe_fd[1]);
	close(vare.pipe_fd[0]);
	waitpid(pid[0], &vare.status, 0);
	waitpid(pid[1], &vare.status, 0);
	free_all(&vare);
	return (0);
}

int	loop(t_vare *vare, char **envp, char **argv, pid_t pid)
{
	int	n;

	n = 0;
	if (vare->i == 0 && pid == 0)
	{
		n = child1(vare, envp, vare->pipe_fd, argv);
		if (n == 1)
			return (1);
		if (n == 2)
			return (2);
	}
	if (vare->i == 1 && pid == 0)
	{
		n = child2(vare, envp, vare->pipe_fd, argv);
		if (n == 1)
			return (1);
		if (n == 2)
			return (2);
	}
	return (0);
}

int	child1(t_vare *vare, char **envp, int *pipe_fd, char **argv)
{
	vare->fd_in = open(argv[1], O_RDONLY);
	if (vare->fd_in < 0)
		return (write_error_2(argv[1], 2), 0);
	if (dup2(vare->fd_in, STDIN_FILENO) < 0)
		return (write_error("dup2", vare, "1111"), 1);
	if (close(vare->fd_in) < 0)
		return (write_error("fd_in", vare, "0111"), 1);
	// if (close(vare->fd_out) < 0)
	// 	return (write_error("fd_out", vare, "0011"), 1);
	if (close(pipe_fd[0]) < 0)
		return (write_error("pipe_fd[0]", vare, "0010"), 1);
	if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		return (write_error("dup2", vare, "0010"), 1);
	if (close(pipe_fd[1]) < 0)
		return (write_error("pipe_fd[1]", vare, "0000"), 1);
	if (raccourci(argv, vare, 2) == 1)
	{
		vare->the_path = get_the_path(envp, vare->cmd1, vare);
		if (vare->the_path == NULL)
			return (write_error_2(argv[2], 1), 2);
	}
	if (execve(vare->the_path, vare->cmd1, envp) < 0)
	{
		free_all(vare);
		return (write_error("execve", vare, "0000"), 1);
	}
	return (1);
}

int	child2(t_vare *vare, char **envp, int *pipe_fd, char **argv)
{
	vare->fd_out = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);//secu
	if (vare->fd_out < 0)
		return (write_error(argv[4], vare, "1000"), 1);
	if (dup2(vare->fd_out, STDOUT_FILENO) < 0)
		return (write_error("dup2", vare, "1111"), 1);
	if (close(vare->fd_out) < 0)
		return (write_error("fd_out", vare, "1011"), 1);
	// if (close(vare->fd_in) < 0)
	// 	return (write_error("fd_in", vare, "0011"), 1);
	if (close(pipe_fd[1]) < 0)
		return (write_error("pipe_fd[1]", vare, "0001"), 1);
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		return (write_error("dup2", vare, "0000"), 1);
	if (close(pipe_fd[0]) < 0)
		return (write_error("pipe[0]", vare, "0000"), 1);
	if (raccourci(argv, vare, 3) == 1)
	{
		vare->the_path = get_the_path(envp, vare->cmd2, vare);
		if (vare->the_path == NULL)
			return (write_error_2(argv[3], 1), 2);
	}
	if (execve(vare->the_path, vare->cmd2, envp) < 0)
	{
		free_all(vare);
		write_error("execve", vare, "0000");
	}
	return (1);//ne renvoie rien si tout ce passe bien
}
