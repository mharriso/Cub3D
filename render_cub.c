/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_cub.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 18:26:22 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/27 21:20:56 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	ray_cast(t_cub *cub, float a, int i)
{
	cub->wall.rays[i] = 0;
	cub->wall.ray_x = cub->player.pos_x + cub->wall.rays[i] * cos(a);
	cub->wall.ray_y = cub->player.pos_y + cub->wall.rays[i] * sin(a);
	while (1)
	{
		cub->wall.ray_x = cub->player.pos_x + cub->wall.rays[i] * cos(a);
		if (cub->map.map[(int)cub->wall.ray_y][(int)cub->wall.ray_x] == '1')
		{
			if (a < M_PI_2 || a > 3 * M_PI_2)
				cub->wall.type = EAST;
			else if (a > M_PI_2 && a < 3 * M_PI_2)
				cub->wall.type = WEST;
			break ;
		}
		cub->wall.ray_y = cub->player.pos_y + cub->wall.rays[i] * sin(a);
		if (cub->map.map[(int)cub->wall.ray_y][(int)cub->wall.ray_x] == '1')
		{
			if (a > M_PI)
				cub->wall.type = NORTH;
			else if (a < M_PI)
				cub->wall.type = SOUTH;
			break ;
		}
		cub->wall.rays[i] += 0.01F;
	}
}

void		ray_loop(t_cub *cub)
{
	float	a;
	float	step;

	cub->wall.put_x = 0;
	a = cub->player.angle - FOV / 2;
	step = FOV / cub->config.rx;
	while (cub->wall.put_x < cub->config.rx)
	{
		check_angle(&a);
		ray_cast(cub, a, cub->wall.put_x);
		render_wall(cub, a);
		a += step;
		cub->wall.put_x++;
	}
	if (cub->map.spr_amt)
		render_sprites(cub);
}

void		render_cub(t_cub *cub)
{
	cub->wall.rays = malloc(cub->config.rx * sizeof(float));
	if (!(cub->mlx.win = mlx_new_window(cub->mlx.mlx, cub->config.rx, \
		cub->config.ry, "CUB3D")))
		exit_error("Error\nFailed creating mlx window");
	if (!(cub->map.cub3d.img = mlx_new_image(cub->mlx.mlx, cub->config.rx, \
		cub->config.ry)))
		exit_error("Error\nFailed creating mlx image");
	cub->map.cub3d.addr = mlx_get_data_addr(cub->map.cub3d.img, \
	&cub->map.cub3d.bits_per_pixel, &cub->map.cub3d.line_length, \
	&cub->map.cub3d.endian);
	ray_loop(cub);
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, \
	cub->map.cub3d.img, 0, 0);
	mlx_hook(cub->mlx.win, 2, 1L << 0, key_handler, cub);
	mlx_hook(cub->mlx.win, 17, 1L << 17, close_win, &cub->mlx);
	mlx_loop(cub->mlx.mlx);
}
