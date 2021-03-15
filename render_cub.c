/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_cub.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 18:26:22 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/15 22:37:54 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

int		key_handler(int keycode, t_cub *cub)
{
	int	speed;

	speed = 5;
	if (keycode == KEY_ESC)
		close_win(&cub->mlx);
	if (keycode == KEY_W)
		cub->pl->y -= speed;
	if (keycode == KEY_S)
		cub->pl->y += speed;
	if (keycode == KEY_A)
		cub->pl->x -= speed;
	if (keycode == KEY_D)
		cub->pl->x += speed;

	//portal(art->obj, art->vars->rx, art->vars->ry);
	//printf("%d\n", keycode);
	return (0);
}

void	draw_2dmap(t_cub *cub, int size)
{
	int	x;
	int	y;

	y = 0;
	while (y < cub->map.height * size)
	{
		x = 0;
		while (x < ft_strlen(cub->map.map[y/size]) * size)
		{
			if (cub->map.map[(int)(y/size)][(int)(x/size)] == '1')
				my_mlx_pixel_put(&cub->map.map2d, x, y, C_MAGENTA);
			x++;
		}
		y++;
	}
}

void	draw_player(t_img player, int size)
{
	int	x;
	int	y;

	y = 0;
	while(y < size)
	{
		x = 0;
		while(x < size)
		{
			my_mlx_pixel_put(&player, x, y, C_YELLOW);
			x++;
		}
		y++;
	}
}
int		render_next_frame(t_cub *cub)
{
	//mlx_clear_window(cub->mlx.mlx, cub->mlx.win);
	mlx_put_image_to_window(cub->mlx.mlx,  cub->mlx.win, cub->map.map2d.img, 0, 0);
	mlx_put_image_to_window(cub->mlx.mlx,  cub->mlx.win, cub->pl->img, cub->pl->x , cub->pl->y);
	return (0);
}

void	render_cub(t_cub *cub)
{
	t_img p;

	cub->mlx.win = mlx_new_window(cub->mlx.mlx, cub->config.rx, cub->config.ry, "CUB3D");
	cub->map.map2d.img = mlx_new_image(cub->mlx.mlx, cub->config.rx, cub->config.ry);
	if(!(cub->map.map2d.addr = mlx_get_data_addr(cub->map.map2d.img, \
	&cub->map.map2d.bits_per_pixel, &cub->map.map2d.line_length, &cub->map.map2d.endian)))
		exit_error("Error\nFailed");

	int size = 25;
	int	p_size = 20;
	p.img = mlx_new_image(cub->mlx.mlx, p_size, p_size);
	if(!(p.addr = mlx_get_data_addr(p.img, \
	&p.bits_per_pixel, &p.line_length, &p.endian)))
		exit_error("Error\nFailed");


	draw_2dmap(cub, size);
	draw_player(p, p_size);

	//mlx_put_image_to_window(cub->mlx.mlx,  cub->mlx.win, cub->map.map2d.img, 0, 0);

	p.x = cub->player.x * size;
	p.y = cub->player.y * size;
	cub->pl = &p;

	mlx_hook(cub->mlx.win, 2, 1L << 0, key_handler, cub);
	mlx_hook(cub->mlx.win, 17, 1L << 17, close_win, &cub->mlx);
	mlx_loop_hook(cub->mlx.mlx, render_next_frame, cub);
	mlx_loop(cub->mlx.mlx);
}
