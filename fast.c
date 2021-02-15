#include <mlx.h>
#include <stdio.h>

#define TRANSP 0xFF000000
#define RED    0x00FF0000
#define GREEN  0x0000FF00
#define BLUE   0x000000FF


typedef struct  s_data {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
}               t_data;
void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

int             main(void)
{
    void    *mlx;
    void    *mlx_win;
    t_data  img;

    mlx = mlx_init();
	int n = 6;
	int rx = 255 * n;
	int ry = 700;
    mlx_win = mlx_new_window(mlx, rx, ry, "Hello world!");
    img.img = mlx_new_image(mlx, rx, ry);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                 &img.endian);
	int x;
	//int y = 5;

	int color;

	int i = (0x000000FF / 255) << 24;



	for(int y = 0; y < ry; y++)
	{
		x = 0;
		color = RED;
		for(; x < rx / n; x++)
			my_mlx_pixel_put(&img, x, y, color += (int)i);
		color = BLUE + TRANSP;
		for(; x < rx / n * 2; x++)
			my_mlx_pixel_put(&img, x, y, color -= (int)i);
		color = BLUE;
		for(; x < rx / n * 3; x++)
			my_mlx_pixel_put(&img, x, y, color += (int)i);
		color = BLUE + RED + TRANSP;
		for(; x < rx / n * 4; x++)
			my_mlx_pixel_put(&img, x, y, color -= (int)i);
		color = BLUE + RED;
		for(; x < rx / n * 5; x++)
			my_mlx_pixel_put(&img, x, y, color += (int)i);
		color = GREEN + TRANSP;
		for(; x < rx; x++)
			my_mlx_pixel_put(&img, x, y, color -= (int)i);

	}


    mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
    mlx_loop(mlx);
}
