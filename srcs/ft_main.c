/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zakariyahamdouchi <zakariyahamdouchi@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 19:16:08 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/11/23 17:23:30 by zakariyaham      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_list	list;
	pid_t pid;
	int	i;
	int	pipe_fd[2];
	int status;

	// creer un fichier out ? mettre les valeur malloc a NULL
	if (argc != 5)
		return (1);
	list.fd_in = open(argv[1], O_RDWR);//secu, faire une secu pour les 3 open ?
	if (list.fd_in < 0)
		return (1);
	list.fd_out = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC);//secu
	if (list.fd_out < 0)
		return (1);
	status = 0;
	//on doit pouvoir lire le premier fichier qu'on nous envoie, il faut verifier qu'on a le droit de lire dessus sinon envoyer une erreur et faire la deuxieme commande si possible
	list.cmd1 = ft_split(argv[2], ' ');//cmd[1] correspond a la command et le reste aux options, que faire si il commence par un " "
	list.cmd2 = ft_split(argv[3], ' ');//cmd[1] correspond a la command et le reste aux options, que faire si il commence par un " "
	i = 0;
	if (pipe(pipe_fd) < 0 )
		return(ft_printf("Error"), 1);
	while (i < 2)
	{
		pid = fork();
		if (pid < 0)
			return(ft_printf("Error"), 1);
		if (i == 0 && pid ==0)
		{
			dup2(list.fd_in, STDIN_FILENO);
			close(list.fd_out);
			close(pipe_fd[0]);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
			list.the_path = get_the_path(envp, list.cmd1, list);
			if (list.the_path == NULL)
				return (1);
			execve(list.the_path, list.cmd1, envp);
		}
		if (i == 1 && pid == 0)
		{
			dup2(list.fd_out, STDOUT_FILENO);
			close(list.fd_in);
			close(pipe_fd[1]);
			dup2(pipe_fd[0], STDIN_FILENO);
			close(pipe_fd[0]);
			list.the_path = get_the_path(envp, list.cmd2, list);
			if (list.the_path == NULL)
				return (1);
			execve(list.the_path, list.cmd2, envp);
		}
		i++;
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(list.fd_in);
	close(list.fd_out);
	waitpid(pid, &status, 0);
}
