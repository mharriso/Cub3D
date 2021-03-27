/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sprites.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 20:57:13 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/27 21:20:43 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	calculate_sprite(t_cub *cub, int i, int *x_start, int *y_start)
{
	cub->map.spr_dir = atan2(cub->map.sprites[i].y - cub->player.pos_y, \
						cub->map.sprites[i].x - cub->player.pos_x);
	while (cub->map.spr_dir - cub->player.angle > M_PI)
		cub->map.spr_dir -= 2 * M_PI;
	while (cub->map.spr_dir - cub->player.angle < -M_PI)
		cub->map.spr_dir += 2 * M_PI;
	if (cub->map.spr_dir - cub->player.angle < -M_PI_2 || \
	cub->map.spr_dir - cub->player.angle > M_PI_2)
		return (1);
	cub->map.spr_dst = sqrt(cub->map.sprites[i].dst);
	cub->map.spr_size = (int)(cub->config.ry / cub->map.spr_dst);
	*x_start = -cos(cub->map.spr_dir - cub->player.angle + M_PI_2) * \
	cub->config.rx + cub->config.rx / 2 - cub->map.spr_size / 2;
	*y_start = cub->config.ry / 2 - cub->map.spr_size / 2;
	return (0);
}

static void	get_tex_color(t_cub *cub, t_sprite_rend *sr)
{
	float	a;
	float	b;

	a = (float)cub->map.spr_size / cub->config.sprite.width;
	b = (float)cub->map.spr_size / cub->config.sprite.height;
	sr->color = my_mlx_pixel_get(&cub->config.sprite, \
		(sr->x - sr->x_start) / a, (sr->y - sr->y_start) / b);
	if (sr->color == C_TRANSP)
		sr->color = 0;
}

static void	put_sprite(t_cub *cub, int i)
{
	t_sprite_rend	sr;

	if (calculate_sprite(cub, i, &sr.x_start, &sr.y_start) == 1)
		return ;
	sr.x = sr.x_start;
	while (sr.x < cub->map.spr_size + sr.x_start)
	{
		if (sr.x >= 0 && sr.x < cub->config.rx && \
		cub->wall.rays[sr.x] >= cub->map.spr_dst)
		{
			sr.y = sr.y_start;
			while (sr.y < cub->map.spr_size + sr.y_start)
			{
				if (sr.y >= 0 && sr.y < cub->config.ry)
				{
					get_tex_color(cub, &sr);
					if (sr.color)
						my_mlx_pixel_put(&cub->map.cub3d, sr.x, sr.y, sr.color);
				}
				sr.y++;
			}
		}
		sr.x++;
	}
}

void		render_sprites(t_cub *cub)
{
	int	i;

	i = 0;
	while (i < cub->map.spr_amt)
	{
		cub->map.sprites[i].dst = \
		pow(cub->player.pos_x - cub->map.sprites[i].x, 2) + \
		pow(cub->player.pos_y - cub->map.sprites[i].y, 2);
		i++;
	}
	sort_array(cub->map.sprites, cub->map.spr_amt);
	i = 0;
	while (i < cub->map.spr_amt)
	{
		put_sprite(cub, i);
		i++;
	}
}
