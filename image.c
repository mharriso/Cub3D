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
t_data	resize(t_data img, t_vars *vars, int new_x, int new_y)
{
	t_data	new_img;
	int		color;
	float	a;
	float	b;

	new_img.img = mlx_new_image(vars->mlx,new_x, new_y);
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
int			main()
{
	t_vars	vars;
	t_data	back;
	t_data	new_back;
	t_data	alpaca;
	t_data	new_alpaca;

	int		rx;
	int		ry;

	rx = 1920;
	ry = 1080;
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, rx, ry, "PICTURE");

	back.img = mlx_xpm_file_to_image(vars.mlx, "earth-1080.xpm", &back.width, &back.height);
	back.addr = mlx_get_data_addr(back.img, &back.bits_per_pixel, &back.line_length, &back.endian);

	alpaca.img = mlx_xpm_file_to_image(vars.mlx, "alpaca.xpm", &alpaca.width, &alpaca.height);
	alpaca.addr = mlx_get_data_addr(alpaca.img, &alpaca.bits_per_pixel, &alpaca.line_length, &alpaca.endian);


	new_back = resize(back, &vars, 1920, 1080);
	new_alpaca = resize(alpaca, &vars, alpaca.width / 4, alpaca.height / 4);

	mlx_put_image_to_window(vars.mlx, vars.win, new_back.img, 0, 0);
	mlx_put_image_to_window(vars.mlx, vars.win, new_alpaca.img, 500, 400);

	mlx_hook(vars.win, 2, 1L<<0, close, &vars);
	mlx_loop(vars.mlx);
}
