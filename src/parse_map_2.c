/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 20:49:34 by mharriso          #+#    #+#             */
/*   Updated: 2022/02/20 04:28:15 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	check_cross(t_map *map, int x, int y)
{
	if (map->map[y][x - 1] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (map->map[y][x + 1] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (map->map[y - 1][x] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (map->map[y + 1][x] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (map->map[y - 1][x - 1] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (map->map[y + 1][x + 1] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (map->map[y - 1][x + 1] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (map->map[y + 1][x - 1] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
}

static void	set_sprite(t_sprite *sprites, int x, int y, int i)
{
	sprites[i].x = x + 0.5;
	sprites[i].y = y + 0.5;
}

void		parse_map_2(t_map *map)
{
	int	x;
	int	y;
	int	i;

	if (map->spr_amt)
		if (!(map->sprites = malloc((map->spr_amt + 1) * sizeof(t_sprite))))
			exit_error("Error\nCan not create sprites array");
	y = 1;
	i = map->spr_amt;
	while (y < map->height - 1)
	{
		x = 1;
		while (x < map->width - 1)
		{
			if (ft_strchr(INNER_OBJS, (map->map)[y][x]))
				check_cross(map, x, y);
			if (map->map[y][x] == SPRITE)
				set_sprite(map->sprites, x, y, --i);
			x++;
		}
		y++;
	}
}
