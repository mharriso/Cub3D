/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_cub.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 18:26:22 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/21 20:32:20 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_angle(float *i);
int		render_next_frame(t_cub *cub);
void	draw_2dmap(t_cub *cub);

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
// 				my_mlx_pixel_put(&cub->map.map2d, x, y, C_MAGENTA);
// 			if (cub->map.map[(int)(y/SCALE)][(int)(x/SCALE)] == '0')
// 				my_mlx_pixel_put(&cub->map.map2d, x, y, C_BLUE);
// 			x++;
// 		}
// 		y++;
// 	}
// 	//printf("pos %f %f\n", cub->player.pos_x, cub->player.pos_y);
// 	my_mlx_pixel_put(&cub->map.map2d, cub->player.pos_x, cub->player.pos_y, C_BLUE);
// 	ray_loop(cub);
// 	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->map.map2d.img, 0, 0);
// }

// void	draw_player(t_cub *cub)
// {
// 	my_mlx_pixel_put(&cub->map.map2d, cub->player.pos_x, cub->player.pos_y, C_BLUE);

// }


// int		render_next_frame(t_cub *cub)
// {
// 	//mlx_clear_window(cub->mlx.mlx, cub->mlx.win);
// 	ray_loop(cub);
// 	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->map.map2d.img, 0, 0);
// 	return (0);
// }

float	ray_cast(t_cub *cub, float a)
{
	float c;

	c = 0;
	check_angle(&a);
	cub->map.ray_x = cub->player.pos_x + c * cos(a);
	cub->map.ray_y = cub->player.pos_y + c * sin(a);
	while(1)
	{
		cub->map.ray_x = cub->player.pos_x + c * cos(a);
		if(cub->map.map[(int)cub->map.ray_y][(int)cub->map.ray_x] == '1') // E W
		{
			if(a < M_PI_2 || a > 3 * M_PI_2)
				cub->map.wall_type = EAST;
			else if(a > M_PI_2 && a < 3 * M_PI_2)
				cub->map.wall_type = WEST;
			break ;
			/* int color = my_mlx_pixel_get(&texture, (y - (int)y) * width_texture, y_ray_loop / b */
		}

		cub->map.ray_y = cub->player.pos_y + c * sin(a);
		if(cub->map.map[(int)cub->map.ray_y][(int)cub->map.ray_x] == '1') // S N
		{
			if(a > M_PI)
				cub->map.wall_type = SOUTH;
			else if(a < M_PI)
				cub->map.wall_type = NORTH;
			// b = wall_height / texture_height
			/* int color = my_mlx_pixel_get(&texture, (x - (int)x) * width_texture, y_ray_loop / b */
			break ;
		}

		c += 0.01F;
	}
	return (c);
}
void	put_texture(t_cub *cub, t_img *texture, int get_x, int put_y, int put_x, int start)
{
	int get_y;
	int color;
	float d;

	d = (float)cub->map.wall_height / (float)texture->height;
	//printf("t_h = %d, w_h = %d\n", texture->height, cub->map.wall_height);
	get_y = (put_y - start) / d;
	//printf("get_y = %d, put_y = %d\n", get_y, put_y);
	color = my_mlx_pixel_get(texture, get_x, get_y);
	my_mlx_pixel_put(&cub->map.map2d, put_x, put_y, color);
}
void	ray_loop(t_cub *cub)
{
	int		x;
	int		y;
	float	a;
	float	step;
	float	wall_d;
	int		wall_start;
	int		wall_end;
	float	k_y;
	float	k_x;

	x = 0;
	a = cub->player.angle - FOV / 2;
	step = FOV / cub->config.rx;
	while(x < cub->config.rx)
	{
		y = 0;
		wall_d = ray_cast(cub, a);
		if(wall_d)
			cub->map.wall_height = cub->config.ry / (wall_d * cos(a - cub->player.angle));
		else
			cub->map.wall_height = cub->config.ry;
		wall_start = cub->config.ry / 2 - cub->map.wall_height / 2;
		wall_end = cub->config.ry - wall_start;
		//printf("wall_d = %d cub->map.wall_height = %d  s %d e %d\n", wall_d, cub->map.wall_height, wall_start, wall_end);
		while(y < wall_start)
		{
			my_mlx_pixel_put(&cub->map.map2d, x, y, cub->config.ceiling);
			y++;
		}
		k_x = (cub->map.ray_x - (int)cub->map.ray_x);
		k_y = (cub->map.ray_y - (int)cub->map.ray_y);
		//printf("k_x = %f k_y = %f\n", k_x, k_y);
		while(y < wall_end)
		{
			if(cub->map.wall_type == NORTH)
				put_texture(cub, &cub->config.north, k_x * cub->config.north.width, y, x, wall_start);
				//my_mlx_pixel_put(&cub->map.map2d, x, y, C_MAGENTA);
			else if(cub->map.wall_type == SOUTH)
				put_texture(cub, &cub->config.south, k_x * cub->config.south.width, y, x, wall_start);
				//my_mlx_pixel_put(&cub->map.map2d, x, y, C_YELLOW);
			else if(cub->map.wall_type == EAST)
				put_texture(cub, &cub->config.east, k_y * cub->config.east.width, y, x, wall_start);
				//my_mlx_pixel_put(&cub->map.map2d, x, y, C_GREEN);
			else if(cub->map.wall_type == WEST)
				put_texture(cub, &cub->config.west, k_y * cub->config.west.width, y, x, wall_start);
				//my_mlx_pixel_put(&cub->map.map2d, x, y, C_BLUE);
			y++;
		}
		while(y < cub->config.ry)
		{
			my_mlx_pixel_put(&cub->map.map2d, x, y, cub->config.floor);
			y++;
		}
		a += step;
		x++;
	}
}
int		key_handler(int keycode, t_cub *cub)
{
	float	speed;
	float	rot;
	int		x;
	int		y;

	speed = 0.3;
	rot = 0.1;

	x = cub->player.pos_x;
	y = cub->player.pos_y;
	if (keycode == KEY_ESC)
		close_win(&cub->mlx);
	if (keycode == KEY_W && cub->map.map[(int)(y + (speed * sin(cub->player.angle)))][(int)(x + (speed * cos(cub->player.angle)))] == '0')
	{
		cub->player.pos_y += speed * sin(cub->player.angle);
		cub->player.pos_x += speed * cos(cub->player.angle);
	}
	// if (keycode == KEY_W && cub->map.map[(int)(y - sin(cub->player.angle))][(int)(x + cos(cub->player.angle))] != '1')
	// {
	// 	printf("%f\n", cub->player.angle);
	// 	printf("x = %f\ny = %f\n",cub->player.pos_x, cub->player.pos_y);
	// 	cub->player.pos_y -= sin(cub->player.angle);
	// 	cub->player.pos_x += cos(cub->player.angle);
	// }
	// if (keycode == KEY_W && cub->map.map[(int)(y - speed)][x] == '0')
	// 	cub->player.pos_y -= speed;
	// if (keycode == KEY_S && cub->map.map[(int)(y + speed)][x] == '0')
	// 	cub->player.pos_y += speed;
	if (keycode == KEY_S && cub->map.map[(int)(y - (speed * sin(cub->player.angle)))][(int)(x - (speed * cos(cub->player.angle)))] == '0')
	{
		cub->player.pos_y -= speed * sin(cub->player.angle);
		cub->player.pos_x -= speed * cos(cub->player.angle);
	}
	if (keycode == KEY_A && cub->map.map[y][(int)(x - speed)] == '0')
		cub->player.pos_x -= speed;
	if (keycode == KEY_D && cub->map.map[y][(int)(x + speed)] == '0')
		cub->player.pos_x += speed;
	if (keycode == KEY_LEFT)
	{
		cub->player.angle -= rot;
		check_angle(&cub->player.angle);
		printf("%f\n", cub->player.angle);
	}
	if (keycode == KEY_RIGHT)
	{
		cub->player.angle += rot;
		check_angle(&cub->player.angle);
		printf("%f\n", cub->player.angle);

	}
	ray_loop(cub);
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->map.map2d.img, 0, 0);
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
	cub->map.map2d.img = mlx_new_image(cub->mlx.mlx, cub->config.rx, cub->config.ry);
	if(!(cub->map.map2d.addr = mlx_get_data_addr(cub->map.map2d.img, \
	&cub->map.map2d.bits_per_pixel, &cub->map.map2d.line_length, &cub->map.map2d.endian)))
		exit_error("Error\nFailed");

	// cub->player.pos_x *= SCALE;
	// cub->player.pos_y *= SCALE;
	//draw_2dmap(cub);
	//draw_player(cub);
	ray_loop(cub);
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->map.map2d.img, 0, 0);
	mlx_hook(cub->mlx.win, 2, 1L << 0, key_handler, cub);
	mlx_hook(cub->mlx.win, 17, 1L << 17, close_win, &cub->mlx);
    //mlx_loop_hook(cub->mlx.mlx, render_next_frame, cub);
	mlx_loop(cub->mlx.mlx);
}
