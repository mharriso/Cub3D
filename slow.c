#include <mlx.h>
#include <stdio.h>
void	heart(void *mlx_ptr, void *win_ptr, int s_x, int s_y, int color)
{
	char heart[13][20] =
    {
	"   *****    *****   ",
    "  ******    ******  ",
    " ********  ******** ",
    "********************",
    " ****************** ",
	"  ****************  ",
    "   **************   ",
	"    ************    ",
    "     **********     ",
	"      ********      ",
    "       ******       ",
	"        ****        ",
    "         **         "
    };
	for(int x = 0; x < 20; x++)
			for(int y = 0; y < 13; y++)
				if(heart[y][x] == '*')
					mlx_pixel_put(mlx_ptr, win_ptr, x + s_x, y + s_y, color);
}

void	big_heart(void *mlx_ptr, void *win_ptr, int s_x, int s_y, int color)
{
	float	dx;
	float	dy;

	dx = (float)s_x / 20;
	dy = (float)s_y / 13;

	char heart[13][20] =
    {
	"   *****    *****   ",
    "  ******    ******  ",
    " ********  ******** ",
    "********************",
    " ****************** ",
	"  ****************  ",
    "   **************   ",
	"    ************    ",
    "     **********     ",
	"      ********      ",
    "       ******       ",
	"        ****        ",
    "         **         "
    };
	for(int x = 0; x < s_x; x++)
			for(int y = 0; y < s_y; y++)
				if(heart[(int)(y / dy)][(int)(x / dx)] == '*')
					mlx_pixel_put(mlx_ptr, win_ptr, x, y, color);
}



int main()
{
	void	*mlx;
	void	*win;


	int rx = 1000;
	int ry = 1000;

	mlx = mlx_init();
	win = mlx_new_window(mlx, rx, ry, "HEART");
	int color = 0x00FF0000;

	// for(int s_y = 5; s_y < 970; s_y += 25)
	// 	for(int s_x = 5; s_x < 970; s_x += 25)
	// 			heart(mlx_ptr, win_ptr, s_x, s_y, color += 2700000);

	big_heart(mlx, win, rx, ry, color);

	mlx_loop(mlx);
}
