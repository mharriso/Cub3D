#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRANSP  0xFF000000
#define RED     0x00FF0000
#define GREEN   0x0000FF00
#define BLUE    0x000000FF
#define YELLOW  0x00FFFF00
#define CYAN    0x0000FFFF
#define MAGENTA 0x00FF00FF
#define WHITE   0x00FFFFFF
#define T       0x01000000
#define R       0x00010000
#define G       0x00000100
#define B       0x00000001
#define RGB		0x00010101

typedef struct  s_data {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
}               t_data;

typedef struct  s_vars {
    void        *mlx;
    void        *win;
}				t_vars;
void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void	handmade(t_data img, int rx, int ry)
{
	int		x;
	int		y;
	int		color;

	for(y = 0; y < ry; y++)
	{
		x = 0;
		color = YELLOW;
		for(; x < 255 ; x++)
			my_mlx_pixel_put(&img, x, y, color += B);
		for(; x < 255 * 2; x++)
			my_mlx_pixel_put(&img, x, y, color -= R);
		for(; x < 255 * 3; x++)
			my_mlx_pixel_put(&img, x, y, color += R);
		for(; x < 255 * 4; x++)
			my_mlx_pixel_put(&img, x, y, color -= G);
		for(; x < 255 * 5; x++)
		{
			color += G;
			my_mlx_pixel_put(&img, x, y, color -= R);
		}
		for(; x < 255 * 6; x++)
		{
			color += R;
			my_mlx_pixel_put(&img, x, y, color -= G);
		}
		for(; x < 255 * 7; x++)
			my_mlx_pixel_put(&img, x, y, color += G);
	}
}
void	mix_stripes(t_data img, int rx, int ry, int size)
{
	int		x;
	int		y;
	int		color;
	int		*colors;
	int		*move;

	srand(time(NULL));
	colors	= malloc(size * sizeof(int));
	move	= malloc(size * sizeof(int));

	for(int i = 0; i < size; i++)
		colors[i] = rand()%0x00FFFFFF;
	for(int i = 0; i < size; i++)
		move[i] = 1 << 8 * (rand()%3);
	for(int y = 0; y < ry; y++)
	{
		x = 0;
		for(int n = 0; n < size; n++)
		{
			color = colors[n];
			for(; x < 255 * (n + 1); x++)
				my_mlx_pixel_put(&img, x, y, color += move[n]);
		}
	}
}
void	mix_cylinders(t_data img, int rx, int ry, int size)
{
	int		x;
	int		y;
	int		color;
	int		*colors;

	srand(time(NULL));
	colors	= malloc(size * sizeof(int));
	for(int i = 0; i < size; i++)
		colors[i] = rand()%0x00FFFFFF;
	for(int y = 0; y < ry; y++)
	{
		x = 0;
		for(int n = 0; n < size; n++)
		{
			if(n%2 == 0)
				color = colors[n] + T;
			else
				color -= T;
			for(; x < 255 * (n + 1); x++)
			{
				if(n%2)
					color += T;
				else
					color -= T;
				my_mlx_pixel_put(&img, x, y, color);
			}
		}
	}
}
void	random_gradient(t_data img, int rx, int ry)
{
	int		x;
	int		y;
	int		color;
	int		buf;
	int		*colors;
	int		move;
	int		tmp;

	srand(time(NULL));
	colors = malloc(7 * sizeof(int));

	colors[0] = RED;
	colors[1] = GREEN;
	colors[2] = BLUE;
	colors[3] = YELLOW;
	colors[4] = CYAN;
	colors[5] = MAGENTA;
	colors[6] = WHITE;

	color = colors[rand()%7];
	move  = RGB - color & RGB;
	tmp = color;
	printf("%08x\n", color);
	printf("%08x\n", move);
	printf("%08x\n", tmp);
	for(y = 0; y < ry; y++)
	{
		color = tmp;
		for(x = 0; x < rx ; x++)
		{
				if(x == 0)
					tmp = color;
				my_mlx_pixel_put(&img, x, y, color += move);
		}
	}

}
void	mix(t_data img, int rx, int ry, int size)
{
	int		x;
	int		y;
	int		color;
	int		*colors;
	int		*pallete;
	int		*move;

	srand(time(NULL));
	colors = malloc(7 * sizeof(int));
	pallete = malloc(7 * sizeof(int));

	pallete[0] = RED;
	pallete[1] = GREEN;
	pallete[2] = BLUE;
	pallete[3] = YELLOW;
	pallete[4] = CYAN;
	pallete[5] = MAGENTA;
	pallete[6] = WHITE;

	move	= malloc(size * sizeof(int));

	for(int i = 0; i < size; i++)
	{
		if(i % 2)
			colors[i] = WHITE;
		else
			colors[i] =  pallete[rand()%7];
	}
	for(int i = 0; i < size; i++)
	{
		if(i % 2)
			move[i] = -(1 << 8 * (rand()%3));
		else
			move[i] = RGB - colors[i] & RGB;
	}
	for(int y = 0; y < ry; y++)
	{
		x = 0;
		//for(int x = 0; x < rx; x++)
		for(int n = 0; n < size; n++)
		{
			for(; x < 255 * (n + 1); x++)
				my_mlx_pixel_put(&img, x, y, color += move[n]);
		}
	}
}
void	gradient(t_data img, int rx, int ry, int size)
{
	int		color;
	int		move;

	for(int y = 0; y < ry; y++)
	{
		color = 0;
		for(int x = 0; x < rx; x++)
		{

			if(color == WHITE)
				move = -1;
			if(color == 0)
				move = 1;
			color += move;
			if(move == 1)
			{
				if(color >= BLUE)
					color |= BLUE;
				if(color >= CYAN)
					color |= CYAN;
			}
			if(move == -1)
			{
				if(color <= RED)
					color -= GREEN;
				if(color <= YELLOW)
					color -= BLUE;
			}
			my_mlx_pixel_put(&img, x, y, color);
		}
	}
}

int		close(int keycode, t_vars *vars)
{
	if(keycode == 53)
	{
		//mlx_clear_window(vars->mlx, vars->win);
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	return 0;
}

int			main(int argc, char **argv)
{
	void	*mlx;
	void	*mlx_win;
	t_vars	vars;
	t_data	img;
	int		size;
	int		color;
	int		x;
	int		y;

	srand(time(NULL));
	vars.mlx = mlx_init();
	size = 7;
	if(argc == 2)
		size = atoi(argv[1]);
	int rx = 255 * size;
	int ry = 1200;
	vars.win = mlx_new_window(vars.mlx, rx, ry, "GRADIENTS");
	img.img = mlx_new_image(vars.mlx, rx, ry);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

	//handmade(img, rx, ry);
	//mix_stripes(img, rx, ry, size);
	//mix_cylinders(img, rx, ry, size);
	//random_gradient(img, rx, ry);
	//mix(img, rx, ry, size);
	gradient(img, rx, ry, size);

	// int i = (0x000000FF / 255) << 24;
	// int t = 0x01000000;
	// int r = 0x00010000;
	// int g = 0x00000100;
	// int b = 0x00000001;

	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
	//mlx_key_hook(vars.win, close, &vars);
	mlx_hook(vars.win, 2, 1L<<0, close, &vars);
	mlx_loop(vars.mlx);

}
