/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_cub.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 18:26:22 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/23 01:02:10 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_angle(float *i);
int		render_next_frame(t_cub *cub);
void	draw_2dmap(t_cub *cub);
void	render_sprite(t_cub *cub, int i);


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
	//printf("pixel x = %d y = %d\n", x,y);
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

// t_img	resize(t_img img, void *mlx, int new_x, int new_y)
// {
// 	t_img	new_img;
// 	int		color;
// 	float	a;
// 	float	b;

// 	new_img.img = mlx_new_image(mlx, new_x, new_y);
// 	new_img.addr = mlx_get_data_addr(new_img.img, &new_img.bits_per_pixel, \
// 	&new_img.line_length, &new_img.endian);
// 	a = (float)new_x / img.width;
// 	b = (float)new_y / img.height; //new_y = wall_height
// 	for (int y = 0; y < new_y; y++)
// 		for (int x = 0; x < new_x; x++)
// 		{
// 				color = my_mlx_pixel_get(&img, x / a, y / b);
// 				my_mlx_pixel_put(&new_img, x , y, color);
// 		}
// 	return (new_img);
// }


// void	draw_2dmap(t_cub *cub)
// {
// 	int	x;
// 	int	y;

// 	y = 0;
// 	while (y < cub->map.height * SCALE)
// 	{
// 		x = 0;
// 		while (x < ft_strlen(cub->map.map[y/SCALE]) * SCALE)
// 		{
// 			if (cub->map.map[(int)(y/SCALE)][(int)(x/SCALE)] == '1')
// 				my_mlx_pixel_put(&cub->map.cub3d, x, y, C_MAGENTA);
// 			if (cub->map.map[(int)(y/SCALE)][(int)(x/SCALE)] == '0')
// 				my_mlx_pixel_put(&cub->map.cub3d, x, y, C_BLUE);
// 			x++;
// 		}
// 		y++;
// 	}
// 	//printf("pos %f %f\n", cub->player.pos_x, cub->player.pos_y);
// 	my_mlx_pixel_put(&cub->map.cub3d, cub->player.pos_x, cub->player.pos_y, C_BLUE);
// 	ray_loop(cub);
// 	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->map.cub3d.img, 0, 0);
// }

// void	draw_player(t_cub *cub)
// {
// 	my_mlx_pixel_put(&cub->map.cub3d, cub->player.pos_x, cub->player.pos_y, C_BLUE);

// }


// int		render_next_frame(t_cub *cub)
// {
// 	//mlx_clear_window(cub->mlx.mlx, cub->mlx.win);
// 	ray_loop(cub);
// 	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->map.cub3d.img, 0, 0);
// 	return (0);
// }

float	ray_cast(t_cub *cub, float a)
{
	float c;

	c = 0;
	check_angle(&a);
	cub->map.ray_x = cub->player.pos_x + c * cos(a);
	cub->map.ray_y = cub->player.pos_y + c * sin(a);
	while (1)
	{
		cub->map.ray_x = cub->player.pos_x + c * cos(a);
		if (cub->map.map[(int)cub->map.ray_y][(int)cub->map.ray_x] == '1') // E W
		{
			if (a < M_PI_2 || a > 3 * M_PI_2)
				cub->map.wall_type = EAST;
			else if (a > M_PI_2 && a < 3 * M_PI_2)
				cub->map.wall_type = WEST;
			break ;
		}
		cub->map.ray_y = cub->player.pos_y + c * sin(a);
		if (cub->map.map[(int)cub->map.ray_y][(int)cub->map.ray_x] == '1') // S N
		{
			if (a > M_PI)
				cub->map.wall_type = NORTH;
			else if (a < M_PI)
				cub->map.wall_type = SOUTH;
			break ;
		}
		c += 0.01F;
	}
	return (c);
}

void	put_texture(t_cub *cub, t_img *texture, int get_x)
{
	int get_y;
	int color;
	float d;

	d = (float)cub->map.wall_height / (float)texture->height;
	get_y = (cub->map.put_y - cub->map.wall_start) / d;
	color = my_mlx_pixel_get(texture, get_x, get_y);
	my_mlx_pixel_put(&cub->map.cub3d, cub->map.put_x, cub->map.put_y, color);
}

void	render_wall(t_cub *cub)
{
	while (cub->map.put_y < cub->map.wall_start )
	{
		my_mlx_pixel_put(&cub->map.cub3d, cub->map.put_x, cub->map.put_y, cub->config.ceiling);
		cub->map.put_y++;
	}
	float k_x = (cub->map.ray_x - (int)cub->map.ray_x);
	float k_y = (cub->map.ray_y - (int)cub->map.ray_y);
	while (cub->map.put_y < cub->map.wall_end && cub->map.put_y < cub->config.ry)
	{
		if (cub->map.wall_type == NORTH)
			put_texture(cub, &cub->config.north, k_x * cub->config.north.width);
		else if (cub->map.wall_type == SOUTH)
			put_texture(cub, &cub->config.south, k_x * cub->config.south.width);
		else if (cub->map.wall_type == EAST)
			put_texture(cub, &cub->config.east, k_y * cub->config.east.width);
		else if (cub->map.wall_type == WEST)
			put_texture(cub, &cub->config.west, k_y * cub->config.west.width);
		cub->map.put_y++;
	}
	while (cub->map.put_y < cub->config.ry)
	{
		my_mlx_pixel_put(&cub->map.cub3d, cub->map.put_x, cub->map.put_y, cub->config.floor);
		cub->map.put_y++;
	}
}
void	ray_loop(t_cub *cub)
{
	float	a;
	float	step;
	float	wall_d;
	float	k_y;
	float	k_x;

	cub->map.put_x = 0;
	a = cub->player.angle - FOV / 2;
	step = FOV / cub->config.rx;
	while (cub->map.put_x < cub->config.rx)
	{
		cub->map.put_y = 0;
		wall_d = ray_cast(cub, a);
		if (wall_d > 0)
			cub->map.wall_height = cub->config.ry / (wall_d * cos(a - cub->player.angle));
		else
			cub->map.wall_height = cub->config.ry;
		cub->map.wall_start = cub->config.ry / 2 - cub->map.wall_height / 2;
		cub->map.wall_end = cub->config.ry - cub->map.wall_start;
		render_wall(cub);
		a += step;
		cub->map.put_x++;
	}
	if (cub->map.spr_amt)
		render_sprite(cub, 0);
}
void	render_sprite(t_cub *cub, int i)
{


	float sprite_dir = atan2(cub->map.sprites[i].y - cub->player.pos_y, cub->map.sprites[i].x - cub->player.pos_x);
    // удаление лишних оборотов
    //while (sprite_dir - player.a >  M_PI) sprite_dir -= 2*M_PI;
    //while (sprite_dir - player.a < -M_PI) sprite_dir += 2*M_PI;

    // расстояние от игрока до спрайта
    float sprite_dist = sqrt(pow(cub->player.pos_x - cub->map.sprites[i].x, 2) + pow(cub->player.pos_y - cub->map.sprites[i].y, 2));
	// printf("pl x = %f, y = %f\n", cub->player.pos_x, cub->player.pos_y);
	// printf("sp x = %f, y = %f\n", cub->map.sprites[i].x, cub->map.sprites[i].y);
	// printf("dist = %f\n", sprite_dist);
    int sprite_screen_size = (int)(cub->config.ry/sprite_dist);
    // не забывайте, что 3D вид занимает только половину кадрового буфера,
    // таким образом, fb.w/2 для ширины экрана

    int x_start = (sprite_dir - cub->player.angle)*(cub->config.rx/2)/(FOV) + (cub->config.rx/2)/2 - sprite_screen_size/2;

	//printf("cos = %f\n", cosf(sprite_dir - cub->player.angle));
	//int x_start = cub->config.rx/2 + (cub->config.rx/2 * cosf(sprite_dir - cub->player.angle) * sprite_dist) - sprite_screen_size / 2;

    int y_start = cub->config.ry/2 - sprite_screen_size/2;

	int x = x_start;
	while(x < sprite_screen_size + x_start )//&& x < cub->config.rx / 2)
	{
		int y = y_start;
		/*if (x_start + x < 0 || x_start + x >= cub->config.rx)
		{
			x++;
			continue;
		}*/
		while(y < sprite_screen_size + y_start)
		{
			if (y_start + y < 0 || y_start + y >= cub->config.ry)
			{
				y++;
				continue;
			}
			my_mlx_pixel_put(&cub->map.cub3d, x, y, C_MAGENTA);
			y++;
		}
		x++;
	}
}

void	wasd_handler(int key, t_cub *cub)
{
	float m_sin;
	float m_cos;

	m_sin = SPEED * sin(cub->player.angle);
	m_cos = SPEED * cos(cub->player.angle);
	if (key == KEY_W && cub->map.map[(int)(cub->player.pos_y + m_sin)]
	[(int)(cub->player.pos_x + m_cos)] != '1')
	{
		cub->player.pos_y += m_sin;
		cub->player.pos_x += m_cos;
	}
	if (key == KEY_S && cub->map.map[(int)(cub->player.pos_y - m_sin)]
	[(int)(cub->player.pos_x - m_cos)] != '1')
	{
		cub->player.pos_y -= m_sin;
		cub->player.pos_x -= m_cos;
	}
	if (key == KEY_D && cub->map.map[(int)(cub->player.pos_y + m_cos)]
	[(int)(cub->player.pos_x - m_sin)] != '1')
	{
		cub->player.pos_y += m_cos;
		cub->player.pos_x -= m_sin;
	}
	if (key == KEY_A && cub->map.map[(int)(cub->player.pos_y - m_cos)]
	[(int)(cub->player.pos_x + m_sin)] != '1')
	{
		cub->player.pos_y -= m_cos;
		cub->player.pos_x += m_sin;
	}
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
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->map.cub3d.img, 0, 0);
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

	cub->mlx.win = mlx_new_window(cub->mlx.mlx, cub->config.rx, cub->config.ry, "CUB3D");
	cub->map.cub3d.img = mlx_new_image(cub->mlx.mlx, cub->config.rx, cub->config.ry);
	if (!(cub->map.cub3d.addr = mlx_get_data_addr(cub->map.cub3d.img, &cub->map.cub3d.bits_per_pixel, &cub->map.cub3d.line_length, &cub->map.cub3d.endian)))
		exit_error("Error\nFailed");
	ray_loop(cub);
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->map.cub3d.img, 0, 0);
	mlx_hook(cub->mlx.win, 2, 1L << 0, key_handler, cub);
	mlx_hook(cub->mlx.win, 17, 1L << 17, close_win, &cub->mlx);
    //mlx_loop_hook(cub->mlx.mlx, render_next_frame, cub);
	mlx_loop(cub->mlx.mlx);
}
