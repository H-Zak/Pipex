/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:10:29 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/12/02 19:43:39 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int		check_space(char *argv);
void	write_error(char *argv, t_vare *vare, char *tab);
void	init(t_vare *vare);

int	ft_check_open(char **argv, t_vare *vare, int argc)
{
	if (argc != 5)
		return (ft_putstr_fd("The number of arguments is incorrect\n", 2), 1);
	if (argv[2][0] == '\0' && argv[3][0] == '\0')
	{
		ft_putstr_fd("bash: : command not found\n", 2);
		return (ft_putstr_fd("bash: : command not found\n", 2), 1);
	}
	if (argv[2][0] == '\0' || argv[3][0] == '\0')
		return (ft_putstr_fd("bash: : command not found\n", 2), 1);
	if (pipe(vare->pipe_fd) < 0)
	{
		ft_putstr_fd("bash: ", 2);
		perror("pipe_fd");
		return (free(vare), 1);
	}
	vare->cmd1 = ft_split(argv[2], ' ');
	if (vare->cmd1 == NULL)
		return (ft_putstr_fd("bash: : command not found\n", 2), 1);
	vare->cmd2 = ft_split(argv[3], ' ');
	if (vare->cmd2 == NULL)
		return (ft_putstr_fd("bash: : command not found\n", 2), 1);
	return (0);
}

void	init(t_vare *vare)
{
	vare->the_path = NULL;
	vare->cmd1 = NULL;
	vare->cmd2 = NULL;
	vare->cmd_path = NULL;
	vare->path = NULL;
	vare->status = 0;
	vare->i = 0;
}

int	found_space(char *argv, int i)
{
	int	space;

	space = 0;
	while (argv[i])
	{
		space = 0;
		while (argv[i] == ' ')
		{
			i++;
			space++;
		}
		if (space > 1)
			return (1);
		i++;
	}
	return (0);
}

int	check_space(char *argv)
{
	int	len;
	int	n;

	len = 0;
	n = 0;
	len = ft_strlen(argv);
	if (argv[0] == ' ' || argv[len - 1] == ' ')
		n = 1;
	if (found_space(argv, 0) != 0)
		n = 1;
	return (n);
}

void	write_error(char *argv, t_vare *vare, char *tab)
{
	ft_putstr_fd("bash: ", 2);
	perror(argv);
	if (tab[0] == '1')
		close(vare->fd_in);
	if (tab[1] == '1')
		close(vare->fd_out);
	if (tab[2] == '1')
		close(vare->pipe_fd[1]);
	if (tab[3] == '1')
		close(vare->pipe_fd[0]);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	free_all(vare);
	exit(EXIT_FAILURE);
}
