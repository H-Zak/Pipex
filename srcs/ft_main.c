/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 19:16:08 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/11/24 18:13:49 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int		child1(t_vare *vare, char **envp, int *pipe_fd);
int		child2(t_vare *vare, char **envp, int *pipe_fd);
void	free_all(t_vare *vare);

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

int	main(int argc, char **argv, char **envp)
{
	t_vare	vare;
	pid_t pid;

	if (ft_check_open(argv, &vare, argc) != 0)
		return (free_all(&vare), 1);
	//on doit pouvoir lire le premier fichier qu'on nous envoie, il faut verifier qu'on a le droit de lire dessus sinon envoyer une erreur et faire la deuxieme commande si possible
	while (vare.i < 2)
	{
		pid = fork();
		if (pid < 0)
			return(perror("open"), 1);
		if (vare.i == 0 && pid == 0)
		{
			if (child1(&vare, envp, vare.pipe_fd) == 1)
				return (free_all(&vare), 1);
		}
		if (vare.i == 1 && pid == 0)
		{
			if (child2(&vare, envp, vare.pipe_fd) == 1)
				return (free_all(&vare), 1);
		}
		vare.i++;
	}
	if (end_close(&vare) == 1)
		return (free_all(&vare), 1);
	waitpid(pid, &vare.status, 0);
}

int	child1(t_vare *vare, char **envp, int *pipe_fd)//faire different return pour differencier l'erreur dup2, close et execve
{
	if (dup2(vare->fd_in, STDIN_FILENO) < 0)
		return (perror("dup2"), 1);
	if (close(vare->fd_out) < 0)
		return (perror("close"), 1);
	if (close(pipe_fd[0]) < 0)
		return (perror("close"), 1);
	if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		return (perror("dup2"), 1);
	if (close(pipe_fd[1]) < 0)
		return (perror("close"), 1);
	vare->the_path = get_the_path(envp, vare->cmd1, vare);
	if (vare->the_path == NULL)
		return (ft_putstr_fd("No path found\n", 2), 1);
	if (execve(vare->the_path, vare->cmd1, envp) < 0)//free tous si ca ne marche pas et enoyer perror
	{
		free_all(vare);
		return (perror("execve"), 1);
	}
	return(1);
}

int	child2(t_vare *vare, char **envp, int *pipe_fd)
{
	if (dup2(vare->fd_out, STDOUT_FILENO) < 0)
		return (perror("dup2"), 1);
	if (close(vare->fd_in) < 0)
		return (perror("close"), 1);
	if (close(pipe_fd[1]) < 0)
		return (perror("close"), 1);
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		return (perror("dup2"), 1);
	if (close(pipe_fd[0]) < 0)
		return (perror("close"), 1);
	vare->the_path = get_the_path(envp, vare->cmd2, vare);
	if (vare->the_path == NULL)
		return (ft_putstr_fd("No path found\n", 2), 1);
	if (execve(vare->the_path, vare->cmd2, envp) < 0)
	{
		free_all(vare);
		perror("execve");
	}
	return (1);//ne renvoie rien si tout ce passe bien
}

void free_all(t_vare *vare)//peut etre devoir calculer la taille des char** pour eviter les conditionnal jump
{
	int	i;

	i = 0;
	if (vare->the_path != NULL)
		free(vare->the_path);
	if (vare->path != NULL)
		free(vare->path);
	if (vare->cmd1 != NULL)
	{
		while(vare->cmd1[i])
		{
			free(vare->cmd1[i]);
			i++;
		}
		free(vare->cmd1);
	}
	i = 0;
	if (vare->cmd2 != NULL)
	{
		while(vare->cmd2[i])
		{
			free(vare->cmd2[i]);
			i++;
		}
		free(vare->cmd2);
	}
	i = 0;
	if (vare->cmd_path != NULL)
	{
		while(vare->cmd_path[i])
		{
			free(vare->cmd_path[i]);
			i++;
		}
		free(vare->cmd_path);
	}
}