/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zakariyahamdouchi <zakariyahamdouchi@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 19:16:08 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/11/21 19:43:14 by zakariyaham      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	pid_t pid;
	int	i;
	int	pipe_fd[2];
	char *the_path;
	char **cmd1;
	char **cmd2;
	int status;

	status = 0;
	//on doit pouvoir lire le premier fichier qu'on nous envoie, il faut verifier qu'on a le droit de lire dessus sinon envoyer une erreur et faire la deuxieme commande si possible
	// creer un fichier out ?
	if (argc != 5)
		return (1);
	cmd1 = ft_split(argv[2], ' ');//cmd[1] correspond a la command et le reste aux options, que faire si il commence par un " "
	cmd2 = ft_split(argv[3], ' ');//cmd[1] correspond a la command et le reste aux options, que faire si il commence par un " "
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
			close(pipe_fd[0]);
			dup2(pipe_fd[1], STDOUT);
			close(pipe_fd[1]);
			the_path = get_the_path(envp, cmd1);
			if (the_path == NULL)
				return (1);
			execve(the_path, cmd1, envp);
		}
		if (i == 1 && pid == 0)
		{
			close(pipe_fd[1]);
			dup2(pipe_fd[0], STDIN);
			close(pipe_fd[0]);
			the_path = get_the_path(envp, cmd2);
			if (the_path == NULL)
				return (1);
			execve(the_path, cmd2, envp);
		}
		i++;
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
}
