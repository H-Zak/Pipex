/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:10:29 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/11/28 15:47:23 by zhamdouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	check_space(char *argv);

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
	if (check_space(argv[2]) == 1 && check_space(argv[3]) == 1)
		return (1);
	if (check_space(argv[3]) == 1)
		return (1);
	// if (check_space(argv) == 2)
	// {
	// 	ft_putstr_fd("bash: ", 2);
	// 	ft_putstr_fd(argv[3], 2);
	// 	return (ft_putstr_fd(": command not found\n", 2), 1);
	// }
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

int	found_space(char *argv, int i)
{
	int space;

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

int	check_space(char *argv)//possible conditionnal jump des lors utiliser ft-strlen
{
	int	len;
	int n;

	len = 0;
	n = 0;
	len = ft_strlen(argv);
	if (argv[0] == ' ' || argv[len - 1] == ' ')
		n = 1;
	if (argv[0] == '.' && argv[1] != '/')
		n = 1;
	if(found_space(argv, 0) != 0)
		n = 1;
	if (n == 1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(argv, 2);
		return (ft_putstr_fd(": command not found\n", 2), 1);
	}
	return (0);
}


// int	check_space_2(char **argv)//possible conditionnal jump des lors utiliser ft-strlen
// {
// 	int	len;

// 	len = 0;
// 	len = ft_strlen(argv[2]);
// 	if (argv[2][0] == ' ' || argv[2][len - 1] == ' ')
// 		return (1);
// 	len = ft_strlen(argv[3]);
// 	if (argv[3][0] == ' ' || argv[3][len - 1] == ' ')
// 		return (2);
// 	if (argv[2][0] == '.' && argv[2][1] != '/')
// 		return (1);
// 	if (argv[3][0] == '.' && argv[3][1] != '/')
// 		return (2);
// 	if(found_space(argv, 2, 0) != 0)
// 		return (1);
// 	if(found_space(argv, 3, 0) != 0)
// 		return (2);
// 	return (0);
// }


// int check_cmd(t_vare *vare)
// {
//     if(vare->cmd1[0])
// }