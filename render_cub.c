/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_cub.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 18:26:22 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/13 19:32:01 by mharriso         ###   ########.fr       */
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

	speed = 35;
	if (keycode == KEY_ESC)
		close_win(&cub->mlx);
	// if (keycode == 13)
	// 	art->obj->y -= speed;
	// if (keycode == 1)
	// 	art->obj->y += speed;
	// if (keycode == 0)
	// 	art->obj->x -= speed;
	// if (keycode == 2)
	// 	art->obj->x += speed;
	//portal(art->obj, art->vars->rx, art->vars->ry);
	//printf("%d\n", keycode);
	return (0);
}

void	draw_2dmap(t_cub *cub)
{
	int	x;
	int	y;

	y = 0;
	while (y < cub->map.height)
	{
		x = 0;
		while (x < ft_strlen(cub->map.map[y]))
		{
			if (cub->map.map[y][x] == '1')
				my_mlx_pixel_put(&cub->map.map2d, x / 0.1, y / 0.1, C_MAGENTA);
			x++;
		}
		y++;
	}
}

int		render_next_frame(t_cub *cub)
{
	//mlx_clear_window(cub->mlx.mlx, cub->mlx.win);
	mlx_put_image_to_window(cub->mlx.mlx,  cub->mlx.win, &cub->map.map2d, 0, 0);
	return (0);
}

void	render_cub(t_cub *cub)
{
	printf("!!!!! x = %d y = %d\n", cub->config.rx, cub->config.ry);
	cub->mlx.win = mlx_new_window(cub->mlx.mlx, cub->config.rx, cub->config.ry, "CUB3D");
	cub->map.map2d.img = mlx_new_image(cub->mlx.mlx, cub->config.rx, cub->config.ry);
	if(!(cub->map.map2d.addr = mlx_get_data_addr(cub->map.map2d.img, \
	&cub->map.map2d.bits_per_pixel, &cub->map.map2d.line_length, &cub->map.map2d.endian)))
		exit_error("Error\nFailed");

	draw_2dmap(cub);
	mlx_put_image_to_window(cub->mlx.mlx,  cub->mlx.win, cub->map.map2d.img, 0, 0);



	mlx_hook(cub->mlx.win, 2, 1L << 0, key_handler, cub);
	mlx_hook(cub->mlx.win, 17, 1L << 17, close_win, &cub->mlx);
	//mlx_loop_hook(cub->mlx.mlx, render_next_frame, &cub);
	mlx_loop(cub->mlx.mlx);
}
