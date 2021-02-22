/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/20 19:54:07 by mharriso          #+#    #+#             */
/*   Updated: 2021/02/22 20:48:02 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "art.h"



int		close(int keycode, t_vars *vars)
{
	if(keycode == 53)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	return 0;
}

int		hello(int keycode, t_vars *vars)
{
	dprintf(1, "H E L L O !\n");
	return 0;
}
int		bye(int keycode, t_vars *vars)
{
	dprintf(1, "B Y E !\n");
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

	srand(time(NULL));
	vars.mlx = mlx_init();
	size = atoi(argv[1]);
	//8k
	int rx = 7680;
	int ry = 4320;
	//4k
	rx = 3840;
	ry = 2160;

	rx = 255 * size;
	ry = 255 * 7;

	vars.win = mlx_new_window(vars.mlx, rx, ry, "GRADIENTS");

	img.img = mlx_new_image(vars.mlx, rx, ry);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

	//cmyk(img, rx, ry);
	//mix_stripes(img, rx, ry, size);
	//mix_cylinders(img, rx, ry, size);
	//random_gradient(img, rx, ry);
	//mix(img, rx, ry, size);
	gradient(img, rx, ry, size);

	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);

	mlx_hook(vars.win, 2, 1L<<0, close, &vars);
	mlx_loop(vars.mlx);
}
