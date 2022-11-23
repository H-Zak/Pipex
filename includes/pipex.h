/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zakariyahamdouchi <zakariyahamdouchi@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 19:14:43 by zhamdouc          #+#    #+#             */
/*   Updated: 2022/11/23 17:12:42 by zakariyaham      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>
# include <stddef.h>
# include "./../libs/printf/libftprintf.h"
# include "./../libs/gnl/get_next_line.h"
# include "./../libs/libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>

typedef struct s_list
{
	char	*the_path;
	char	**cmd1;
	char	**cmd2;
	int		fd_in;
	int		fd_out;
	char	**cmd_path;
	char	*path;
}				t_list;
char	*get_the_path(char **envp, char **cmd);

#endif
