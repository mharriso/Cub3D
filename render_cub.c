/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_cub.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 18:26:22 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/25 22:14:08 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_angle(float *i);
int		render_next_frame(t_cub *cub);
void	draw_2dmap(t_cub *cub);
void	render_sprite(t_cub *cub, int i);
void	sort_arr(t_sprite *arr, int n);

int		close_win(t_mlx *mlx)
{
	mlx_destroy_window(mlx->mlx, mlx->win);
	exit(EXIT_SUCCESS);
	return (0);
}

int		my_mlx_pixel_get(t_img *data, int x, int y)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	return (*(unsigned int*)dst);
}

void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	ray_cast(t_cub *cub, float a, int i)
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

void	put_texture(t_cub *cub, t_img *texture, float k)
{
	int		get_y;
	int		get_x;
	int		color;
	float	d;

	d = (float)cub->wall.height / (float)texture->height;
	get_x = k * texture->width;
	get_y = (cub->wall.put_y - cub->wall.start) / d;
	color = my_mlx_pixel_get(texture, get_x, get_y);
	my_mlx_pixel_put(&cub->map.cub3d, cub->wall.put_x, cub->wall.put_y, color);
}

void	render_wall(t_cub *cub)
{
	cub->wall.put_y = 0;
	while (cub->wall.put_y < cub->wall.start)
	{
		my_mlx_pixel_put(&cub->map.cub3d, cub->wall.put_x, \
			cub->wall.put_y, cub->config.ceiling);
		cub->wall.put_y++;
	}
	while (cub->wall.put_y < cub->wall.end && cub->wall.put_y < cub->config.ry)
	{
		if (cub->wall.type == NORTH)
			put_texture(cub, &cub->config.north, cub->wall.k_x);
		else if (cub->wall.type == SOUTH)
			put_texture(cub, &cub->config.south, cub->wall.k_x);
		else if (cub->wall.type == EAST)
			put_texture(cub, &cub->config.east, cub->wall.k_y);
		else if (cub->wall.type == WEST)
			put_texture(cub, &cub->config.west, cub->wall.k_y);
		cub->wall.put_y++;
	}
	while (cub->wall.put_y < cub->config.ry)
	{
		my_mlx_pixel_put(&cub->map.cub3d, cub->wall.put_x, \
		cub->wall.put_y, cub->config.floor);
		cub->wall.put_y++;
	}
}

void	render_sprites(t_cub *cub)
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
	sort_arr(cub->map.sprites, cub->map.spr_amt);
	i = 0;
	while (i < cub->map.spr_amt)
	{
		render_sprite(cub, i);
		i++;
	}
}

void	calculate_wall(t_cub *cub, float a, int i)
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

void	ray_loop(t_cub *cub)
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
		calculate_wall(cub, a, cub->wall.put_x);
		render_wall(cub);
		a += step;
		cub->wall.put_x++;
	}
	if (cub->map.spr_amt)
		render_sprites(cub);
}

void	swap_sprite(t_sprite *a, t_sprite *b)
{
	t_sprite tmp;

	tmp.dst = a->dst;
	tmp.x = a->x;
	tmp.y = a->y;
	a->dst = b->dst;
	a->x = b->x;
	a->y = b->y;
	b->dst = tmp.dst;
	b->x = tmp.x;
	b->y = tmp.y;
}

void	sort_arr(t_sprite *arr, int n)
{
	int		max;
	int		index;
	int		i;
	int		j;

	i = -1;
	while (++i < n)
	{
		max = arr[i].dst;
		index = -1;
		j = i;
		while (++j < n)
		{
			if (arr[j].dst > max)
			{
				max = arr[j].dst;
				index = j;
			}
		}
		if (index != -1)
			swap_sprite(&(arr[i]), &(arr[index]));
	}
}

int		calculate_sprite(t_cub *cub, int i, int *x_start, int *y_start)
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

void	render_sprite(t_cub *cub, int i)
{
	int		x;
	int		y;
	int		x_start;
	int		y_start;

	if (calculate_sprite(cub, i, &x_start, &y_start) == 1)
		return ;
	x = x_start;
	while (x < cub->map.spr_size + x_start)
	{
		if (x >= 0 && x < cub->config.rx && \
		cub->wall.rays[x] >= cub->map.spr_dst)
		{
			y = y_start;
			while (y < cub->map.spr_size + y_start)
			{
				if (y >= 0 && y < cub->config.ry)
					my_mlx_pixel_put(&cub->map.cub3d, x, y, C_MAGENTA);
				y++;
			}
		}
		x++;
	}
}

void	move(t_player *player, float y, float x)
{
	player->pos_y += y;
	player->pos_x += x;
}

void	wasd_handler(int key, t_cub *cub)
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

int		key_handler(int key, t_cub *cub)
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

void	check_angle(float *a)
{
	if (*a > (float)M_PI * 2)
		*a -= (float)M_PI * 2;
	else if (*a < 0)
		*a += (float)2 * M_PI;
}

void	render_cub(t_cub *cub)
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
