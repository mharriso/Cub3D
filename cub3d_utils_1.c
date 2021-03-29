/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 19:56:59 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/29 19:40:38 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define FD STDOUT_FILENO

int		close_win(t_mlx *mlx)
{
	mlx_destroy_window(mlx->mlx, mlx->win);
	exit(EXIT_SUCCESS);
	return (0);
}

void	exit_error(char *s)
{
	ft_putstr_fd(RED, FD);
	if (errno)
		perror(s);
	else
		ft_putendl_fd(s, FD);
	ft_putstr_fd(RESET, FD);
	exit(EXIT_FAILURE);
}

char	**free_2d_array(char **array)
{
	size_t		i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

void	shadow_effect(int k, int *color)
{
	if ((*color & C_BLUE) > k)
		*color -= k;
	else
		*color &= C_YELLOW;
	if ((*color & C_GREEN) > (k << 8))
		*color -= (k << 8);
	else
		*color &= C_MAGENTA;
	if ((*color & C_RED) > (k << 16))
		*color -= (k << 16);
	else
		*color &= C_CYAN;
}
