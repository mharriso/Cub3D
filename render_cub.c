/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_cub.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 18:26:22 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/19 00:05:17 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
// 	b = (float)new_y / img.height;
// 	for (int y = 0; y < new_y; y++)
// 		for (int x = 0; x < new_x; x++)
// 		{
// 				color = my_mlx_pixel_get(&img, x / a, y / b);
// 				my_mlx_pixel_put(&new_img, x , y, color);
// 		}
// 	return (new_img);
// }
int		render_next_frame(t_cub *cub);


float	ray_cast(t_cub *cub, float a)
{
	float c;
	float x;
	float y;

	c = 0;
	x = cub->player.posX + c * cos(a);
	y = cub->player.posY + c * sin(a);
	while(cub->map.map[(int)y][(int)x] != '1')
	{
		x = cub->player.posX + c * cos(a);
		y = cub->player.posY + c * sin(a);
		c += 0.05F;
	}
	return (c);
}

void	ray_loop(t_cub *cub)
{
	int		x;
	int		y;
	float	a;
	float	step;
	int		wall_d;
	int		wall_y;
	int		wall_start;
	int		wall_end;

	x = 0;
	a = cub->player.angle - FOV / 2;
	step = FOV / cub->config.rx;
	while(x < cub->config.rx)
	{
		y = 0;
		wall_d = ray_cast(cub, a);
		if(wall_d)
			wall_y = cub->config.ry / (wall_d * cos(a - cub->player.angle));
		else
			wall_y = cub->config.ry;
		//column_height = win_h/(t*cos(angle-player_a));
		wall_start = cub->config.ry / 2 - wall_y / 2;
		wall_end = cub->config.ry - wall_start;
		printf("wall_d = %d wall_y = %d  s %d e %d\n", wall_d, wall_y, wall_start, wall_end);
		while(y < wall_start)
		{
			my_mlx_pixel_put(&cub->map.map2d, x, y, cub->config.ceiling);
			y++;
		}
		while(y < wall_end)
		{
			my_mlx_pixel_put(&cub->map.map2d, x, y, C_MAGENTA);
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
void	draw_2dmap(t_cub *cub)
{
	int	x;
	int	y;

	y = 0;
	while (y < cub->map.height * SCALE)
	{
		x = 0;
		while (x < ft_strlen(cub->map.map[y/SCALE]) * SCALE)
		{
			if (cub->map.map[(int)(y/SCALE)][(int)(x/SCALE)] == '1')
				my_mlx_pixel_put(&cub->map.map2d, x, y, C_MAGENTA);
			if (cub->map.map[(int)(y/SCALE)][(int)(x/SCALE)] == '0')
				my_mlx_pixel_put(&cub->map.map2d, x, y, C_BLUE);
			x++;
		}
		y++;
	}
	//printf("pos %f %f\n", cub->player.posX, cub->player.posY);
	my_mlx_pixel_put(&cub->map.map2d, cub->player.posX, cub->player.posY, C_BLUE);
	//ray_cast(cub);
	ray_loop(cub);
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->map.map2d.img, 0, 0);
}



// void	draw_player(t_cub *cub)
// {
// 	my_mlx_pixel_put(&cub->map.map2d, cub->player.posX, cub->player.posY, C_BLUE);

// }


int		render_next_frame(t_cub *cub)
{
	//mlx_clear_window(cub->mlx.mlx, cub->mlx.win);
	ray_loop(cub);
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->map.map2d.img, 0, 0);
	return (0);
}

int		key_handler(int keycode, t_cub *cub)
{
	float	speed;
	float	rot;
	int		x;
	int		y;

	speed = 0.5;
	rot = 0.1;

	x = cub->player.posX;
	y = cub->player.posY;
	if (keycode == KEY_ESC)
		close_win(&cub->mlx);
	// if (keycode == KEY_W && cub->map.map[(int)(y - speed)][x] == '0')
	// {
	// 	cub->player.posY += speed * sin(cub->player.angle);
	// 	cub->player.posX += speed * cos(cub->player.angle);
	// }
	if (keycode == KEY_W && cub->map.map[(int)(y - speed)][x] == '0')
		cub->player.posY -= speed;
	if (keycode == KEY_S && cub->map.map[(int)(y + speed)][x] == '0')
		cub->player.posY += speed;
	if (keycode == KEY_A && cub->map.map[y][(int)(x - speed)] == '0')
		cub->player.posX -= speed;
	if (keycode == KEY_D && cub->map.map[y][(int)(x + speed)] == '0')
		cub->player.posX += speed;
	if (keycode == KEY_LEFT)
		cub->player.angle -= rot;
	if (keycode == KEY_RIGHT)
		cub->player.angle += rot;
	//printf("key %d\n", keycode);
	//draw_2dmap(cub);
	ray_loop(cub);
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->map.map2d.img, 0, 0);
	return (0);
}

void	render_cub(t_cub *cub)
{

	cub->mlx.win = mlx_new_window(cub->mlx.mlx, cub->config.rx, cub->config.ry, "CUB3D");
	cub->map.map2d.img = mlx_new_image(cub->mlx.mlx, cub->config.rx, cub->config.ry);
	if(!(cub->map.map2d.addr = mlx_get_data_addr(cub->map.map2d.img, \
	&cub->map.map2d.bits_per_pixel, &cub->map.map2d.line_length, &cub->map.map2d.endian)))
		exit_error("Error\nFailed");

	// cub->player.posX *= SCALE;
	// cub->player.posY *= SCALE;
	//draw_2dmap(cub);
	//draw_player(cub);

	mlx_hook(cub->mlx.win, 2, 1L << 0, key_handler, cub);
	mlx_hook(cub->mlx.win, 17, 1L << 17, close_win, &cub->mlx);
    //mlx_loop_hook(cub->mlx.mlx, render_next_frame, cub);
	mlx_loop(cub->mlx.mlx);
}
