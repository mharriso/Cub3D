/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_wall.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 20:56:27 by mharriso          #+#    #+#             */
/*   Updated: 2021/04/01 22:23:03 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void		put_texture_wall(t_cub *cub, t_img *tex, float k, int mirror)
{
	int		get_y;
	int		get_x;
	int		color;
	float	d;

	if (mirror)
		get_x = tex->width - k * tex->width;
	else
		get_x = k * tex->width;
	d = (float)cub->wall.height / (float)tex->height;
	get_y = (cub->wall.put_y - cub->wall.start) / d;
	color = my_mlx_pixel_get(tex, get_x, get_y);
	shadow_effect(cub->wall.rays[cub->wall.put_x] * SHADOW, &color);
	my_mlx_pixel_put(&cub->map.cub3d, cub->wall.put_x, cub->wall.put_y, color);
}

static void		calculate_wall(t_cub *cub, float a, int i)
{
	if (cub->wall.rays[i] > 0)
		cub->wall.height = cub->config.ry / (cub->wall.rays[i] * \
		cos(a - cub->player.angle));
	else
		cub->wall.height = cub->config.ry;
	cub->wall.start = cub->config.ry / 2 - cub->wall.height / 2;
	cub->wall.end = cub->config.ry - cub->wall.start;
	cub->wall.k_x = (cub->wall.ray_x - (int)cub->wall.ray_x);
	cub->wall.k_y = (cub->wall.ray_y - (int)cub->wall.ray_y);
}

static void		render_ceiling(t_cub *cub)
{
	while (cub->wall.put_y < cub->wall.start)
	{
		if (cub->wall.put_y % SHADOW_F == 1)
			shadow_effect(1, &cub->wall.color);
		my_mlx_pixel_put(&cub->map.cub3d, cub->wall.put_x, \
			cub->wall.put_y, cub->wall.color);
		cub->wall.put_y++;
	}
}

static void		render_floor(t_cub *cub)
{
	int i;

	i = cub->config.ry;
	while (i > cub->wall.put_y)
	{
		i--;
		if (i % SHADOW_C == 1)
			shadow_effect(1, &cub->wall.color);
		my_mlx_pixel_put(&cub->map.cub3d, cub->wall.put_x, \
		i, cub->wall.color);
	}
}

void			render_wall(t_cub *cub, float a)
{
	calculate_wall(cub, a, cub->wall.put_x);
	cub->wall.put_y = 0;
	cub->wall.color = cub->config.ceiling;
	render_ceiling(cub);
	while (cub->wall.put_y < cub->wall.end && cub->wall.put_y < cub->config.ry)
	{
		if (cub->wall.type == NORTH)
			put_texture_wall(cub, &cub->config.north, cub->wall.k_x, 0);
		else if (cub->wall.type == SOUTH)
			put_texture_wall(cub, &cub->config.south, cub->wall.k_x, 1);
		else if (cub->wall.type == EAST)
			put_texture_wall(cub, &cub->config.east, cub->wall.k_y, 0);
		else if (cub->wall.type == WEST)
			put_texture_wall(cub, &cub->config.west, cub->wall.k_y, 1);
		cub->wall.put_y++;
	}
	cub->wall.color = cub->config.floor;
	render_floor(cub);
}
