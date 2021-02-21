#include "art.h"

int		invert_color(int color)
{
	return (~color & WHITE) | (color & TRANSP);
}

void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

int            my_mlx_pixel_get(t_data *data, int x, int y)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    return (*(unsigned int*)dst);
}


void	cmyk(t_data img, int rx, int ry)
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
		colors[i] = rand()%WHITE;
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
void	gradient0(t_data img, int rx, int ry, int size)
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

void	gradient(t_data img, int rx, int ry, int size)
{
	int		color;
	int		move;

	color = 1;
	for(int y = 0; y < ry; y++)
	{
		//color = 1;
		move = 1;
		if((color & BLUE) == BLUE)
			move = G;
		// if((color & GREEN) == GREEN)
		// 	move = 1;
		// if((color & RED) == RED)
		// 	move = 1;
		if((color & CYAN) == CYAN)
			move = R;
		if(color == WHITE -1)
			move = -R;
		color += move;
		for(int x = 0; x < rx; x++)
		{
			if(y == 0 && x > 510)
			{
				printf("%08x\n%08x\n", color, move);
			}
			my_mlx_pixel_put(&img, x, y, color);
		}
	}
}
