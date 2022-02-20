/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 20:44:48 by mharriso          #+#    #+#             */
/*   Updated: 2022/02/20 04:28:06 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	set_player(t_cub *cub, char dir, int x, int y)
{
	if (cub->player.pos_x != 0)
		exit_error("Error\nMany players! Need only one");
	cub->player.pos_x = x + 0.5;
	cub->player.pos_y = y + 0.5;
	cub->map.map[y][x] = '0';
	if (dir == 'N')
		cub->player.angle = 3 * M_PI_2;
	else if (dir == 'S')
		cub->player.angle = M_PI_2;
	else if (dir == 'E')
		cub->player.angle = 0;
	else if (dir == 'W')
		cub->player.angle = M_PI;
}

static void	check_border(t_map *map, int x, int y)
{
	if (x == map->width - 1 || y == map->height - 1)
		exit_error("Error\nInvalid map. Gap detected!");
	if (x == 0 || y == 0)
		exit_error("Error\nInvalid map. Gap detected!");
}

void		parse_map_1(t_cub *cub)
{
	int	x;
	int	y;

	y = 0;
	while (y < cub->map.height)
	{
		x = 0;
		while (x < cub->map.width)
		{
			if (!ft_strchr(VALID_OBJS, cub->map.map[y][x]))
				exit_error("Error\nInvalid map. Invalid object detected!");
			if (ft_strchr(INNER_OBJS, cub->map.map[y][x]))
				check_border(&cub->map, x, y);
			if (ft_strchr(PLAYER, cub->map.map[y][x]))
				set_player(cub, cub->map.map[y][x], x, y);
			if (cub->map.map[y][x] == SPRITE)
				cub->map.spr_amt++;
			x++;
		}
		y++;
	}
	if (cub->player.pos_x == 0)
		exit_error("Error\nMissing player position");
}
