/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/20 19:54:07 by mharriso          #+#    #+#             */
/*   Updated: 2021/04/01 21:51:41 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "art.h"
# define C_TRANSP  0xFF000000
# define C_RED     0x00FF0000
# define C_GREEN   0x0000FF00
# define C_BLUE    0x000000FF
# define C_YELLOW  0x00FFFF00
# define C_CYAN    0x0000FFFF
# define C_MAGENTA 0x00FF00FF
# define C_WHITE   0x00FFFFFF
# define C_BLACK   0x00000000
# define C_T       0x01000000
# define C_R       0x00010000
# define C_G       0x00000100
# define C_B       0x00000001


int		close(int keycode, t_vars *vars)
{
	if (keycode == 53)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	return 0;
}

void	shadow_effect(int k, int *color)
{
	if ((*color & C_BLUE) > k)
		*color -= k;
	else
		*color &= C_YELLOW;
	if ((*color & C_GREEN) > (k << 8))
		*color -= (k << 8);
	else
		*color &= C_MAGENTA;
	if ((*color & C_RED) > (k << 16))
		*color -= (k << 16);
	else
		*color &= C_CYAN;
}

void shadow(t_img *img, int rx, int ry)
{
	int x;
	int y;
	int k;
	int color;

	x = 0;
	while (x < rx)
	{
		y = 0;
		color = C_YELLOW;
		k = 1;
		while (y < ry)
		{
			if (y%2)
				shadow_effect(k, &color);
			my_mlx_pixel_put(img, x, y, color);
			y++;
		}
		x++;
	}
}

t_img	resize(t_img img, t_vars *vars, int new_x, int new_y)
{
	t_img	new_img;
	int		color;
	float	a;
	float	b;

	new_img.img = mlx_new_image(vars->mlx, new_x, new_y);
	new_img.addr = mlx_get_data_addr(new_img.img, &new_img.bits_per_pixel, &new_img.line_length, &new_img.endian);
	a = (float)new_x / img.width;
	b = (float)new_y / img.height;
	for(int y = 0; y < new_y; y++)
		for(int x = 0; x < new_x; x++)
		{
				color = my_mlx_pixel_get(&img, x/a, y/b);
				my_mlx_pixel_put(&new_img, x , y, color);
		}
	return new_img;
}

int		render_next_frame(t_art *art)
{
	mlx_clear_window(art->vars->mlx, art->vars->win);
	mlx_put_image_to_window(art->vars->mlx, art->vars->win, art->back->img, 0, 0);
	mlx_put_image_to_window(art->vars->mlx, art->vars->win, art->obj->img, art->obj->x, art->obj->y);
	return 0;
}
int			main(int argc, char **argv)
{
	t_vars	vars;
	t_img	img;
	int		size;
	int		color;
	int		x;
	int		y;
	int		rx;
	int		ry;

	srand(time(NULL));
	vars.mlx = mlx_init();
	rx = 1400;
	ry = 1000;
	size = 5;
	int w,h;
	vars.win = mlx_new_window(vars.mlx, rx, ry, "GRADIENTS");
	img.img = mlx_xpm_file_to_image(vars.mlx, "texture/sprite2.xpm", &w, &h);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

	//shadow(&img, rx, ry);


	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
	mlx_hook(vars.win, 2, 1L<<0, close, &vars);
	mlx_loop(vars.mlx);
}
