/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zakariyahamdouchi <zakariyahamdouchi@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 19:16:08 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/11/20 19:28:16 by zakariyaham      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char**envp)
{
	argc = 0;
	argv = NULL;
	envp = NULL;
	int	fd[2];
	int id;
	if (pipe(fd) < 0 )
		return(ft_printf("Error"));
	id = fork();
	if (id < 0)
	{
		dup2(fd, 1);
		return (ft_printf("Error"));
	}


	ft_printf("Hello world");
}
