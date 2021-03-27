/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 20:41:59 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/27 21:20:22 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define KEY_ESC 53
#define KEY_W 13
#define KEY_A 0
#define KEY_S 1
#define KEY_D 2
#define KEY_LEFT 123
#define KEY_RIGHT 124

static void	move(t_player *player, float y, float x)
{
	player->pos_y += y;
	player->pos_x += x;
}

static void	wasd_handler(int key, t_cub *cub)
{
	float m_sin;
	float m_cos;

	m_sin = SPEED * sin(cub->player.angle);
	m_cos = SPEED * cos(cub->player.angle);
	if (key == KEY_W && cub->map.map[(int)(cub->player.pos_y + m_sin)]
	[(int)(cub->player.pos_x + m_cos)] == '0')
		move(&cub->player, m_sin, m_cos);
	if (key == KEY_S && cub->map.map[(int)(cub->player.pos_y - m_sin)]
	[(int)(cub->player.pos_x - m_cos)] == '0')
		move(&cub->player, -m_sin, -m_cos);
	if (key == KEY_D && cub->map.map[(int)(cub->player.pos_y + m_cos)]
	[(int)(cub->player.pos_x - m_sin)] == '0')
		move(&cub->player, m_cos, -m_sin);
	if (key == KEY_A && cub->map.map[(int)(cub->player.pos_y - m_cos)]
	[(int)(cub->player.pos_x + m_sin)] == '0')
		move(&cub->player, -m_cos, m_sin);
}

int			key_handler(int key, t_cub *cub)
{
	if (key == KEY_ESC)
		close_win(&cub->mlx);
	wasd_handler(key, cub);
	if (key == KEY_LEFT)
	{
		cub->player.angle -= ROT;
		check_angle(&cub->player.angle);
	}
	if (key == KEY_RIGHT)
	{
		cub->player.angle += ROT;
		check_angle(&cub->player.angle);
	}
	ray_loop(cub);
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, \
	cub->map.cub3d.img, 0, 0);
	return (0);
}
