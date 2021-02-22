#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "art.h"

int		close(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	exit(0);
	return 0;
}
void	portal(t_img *obj, int rx, int ry)
{
	if(obj->y + obj->height <= 0)
		obj->y = ry;
	if(obj->x + obj->width <= 0)
		obj->x = rx;
	if(obj->x > rx)
		obj->x = -obj->width;
	if(obj->y > ry)
		obj->y = -obj->height;
}
int		key_handler(int keycode, t_art *art)
{
	int	speed;

	speed = 35;
	if(keycode == 53)
		close(art->vars);
	if(keycode == 13)
		art->obj->y -= speed;
	if(keycode == 1)
		art->obj->y += speed;
	if(keycode == 0)
		art->obj->x -= speed;
	if(keycode == 2)
		art->obj->x += speed;
	portal(art->obj, art->vars->rx, art->vars->ry);
	return 0;
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
int			main()
{
	t_vars	vars;
	t_img	back;
	t_img	new_back;
	t_img	alpaca;
	t_img	new_alpaca;
	t_art	art;

	srand(time(NULL));
	vars.rx = 1920;
	vars.ry = 1080;
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, vars.rx, vars.ry, "PICTURE");

	back.img = mlx_xpm_file_to_image(vars.mlx, "planet3840x2160.xpm", &back.width, &back.height);
	back.addr = mlx_get_data_addr(back.img, &back.bits_per_pixel, &back.line_length, &back.endian);

	alpaca.img = mlx_xpm_file_to_image(vars.mlx, "alpaca1.xpm", &alpaca.width, &alpaca.height);
	alpaca.addr = mlx_get_data_addr(alpaca.img, &alpaca.bits_per_pixel, &alpaca.line_length, &alpaca.endian);


	new_back = resize(back, &vars, vars.rx, vars.ry);
	new_alpaca = resize(alpaca, &vars, alpaca.width / 2 , alpaca.height / 2);

	new_alpaca.width = alpaca.width / 2;
	new_alpaca.height = alpaca.height / 2;
	new_alpaca.x = new_alpaca.width + rand()%(vars.rx - new_alpaca.width);
	new_alpaca.y = new_alpaca.height + rand()%(vars.ry - new_alpaca.height);

	art.back = &new_back;
	art.obj = &new_alpaca;
	art.vars = &vars;
	//printf("%d %d\n%d %d\n", alpaca.width, alpaca.height, new_alpaca.width, new_alpaca.height);
	mlx_hook(vars.win, 2, 1L<<0, key_handler, &art);
	mlx_hook(vars.win, 17, 1L<<17, close, &vars);
	mlx_loop_hook(vars.mlx, render_next_frame, &art);
	mlx_loop(vars.mlx);
}
