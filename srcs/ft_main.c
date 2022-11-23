/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 19:16:08 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/11/23 21:37:11 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_vare	vare;
	pid_t pid;
	int	i;
	int	pipe_fd[2];
	int status;

	// creer un fichier out ? mettre les valeur malloc a NULL
	if (argc != 5)//verifier que les arguments ne sont pas vides
		return (1);
	if (argv[2][0] == '\0' && argv[3][0] == '\0')
		return (ft_putstr_fd("bash: : command not found\nbash: : command not found\n", 2), 1);
	if (argv[2][0] == '\0' || argv[3][0] == '\0')
		return (ft_putstr_fd("bash: : command not found\n", 2), 1);
	vare.fd_in = open(argv[1], O_RDWR);//secu, faire une secu pour les 3 open ?
	if (vare.fd_in < 0)
		return (1);
	vare.fd_out = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);//secu
	if (vare.fd_out < 0)
	{
		perror("open");
		return (1);
	}
	status = 0;
	//on doit pouvoir lire le premier fichier qu'on nous envoie, il faut verifier qu'on a le droit de lire dessus sinon envoyer une erreur et faire la deuxieme commande si possible
	vare.cmd1 = ft_split(argv[2], ' ');//cmd[1] correspond a la command et le reste aux options, que faire si il commence par un " "
	vare.cmd2 = ft_split(argv[3], ' ');//cmd[1] correspond a la command et le reste aux options, que faire si il commence par un " "
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
			dup2(vare.fd_in, STDIN_FILENO);
			close(vare.fd_out);
			close(pipe_fd[0]);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
			vare.the_path = get_the_path(envp, vare.cmd1, vare);
			if (vare.the_path == NULL)
				return (1);
			execve(vare.the_path, vare.cmd1, envp);//free tous si ca ne marche pas et enoyer perror
		}
		if (i == 1 && pid == 0)
		{
			dup2(vare.fd_out, STDOUT_FILENO);
			close(vare.fd_in);
			close(pipe_fd[1]);
			dup2(pipe_fd[0], STDIN_FILENO);
			close(pipe_fd[0]);
			vare.the_path = get_the_path(envp, vare.cmd2, vare);
			if (vare.the_path == NULL)
				return (1);
			execve(vare.the_path, vare.cmd2, envp);
		}
		i++;
	}
	
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(vare.fd_in);
	close(vare.fd_out);
	waitpid(pid, &status, 0);
}
