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
	int dx;
	int dy;

	dx = (s_x) / 20;
	dy = (s_y) / 13;

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

	/*while (s_y % 13)
	{
		s_y--;
	}
	while (s_x % 20)
	{
		s_x--;
	}*/
	s_x -= s_x % 20;
	s_y -= s_y % 13;

	for(int x = 0; x < s_x; x++)
			for(int y = 0; y < s_y; y++)
				if(heart[y / dy][x / dx] == '*')
					mlx_pixel_put(mlx_ptr, win_ptr, x, y, color);
}


int main()
{
	void *mlx_ptr;
	void *win_ptr;

	int rx = 50;
	int ry = 50;

	mlx_ptr = mlx_init();
	win_ptr = mlx_new_window(mlx_ptr, rx, ry, "HEART");
	int color = 0x00FF0000;

	// for(int s_y = 5; s_y < 970; s_y += 25)
	// 	for(int s_x = 5; s_x < 970; s_x += 25)
	// 	{
	// 			heart(mlx_ptr, win_ptr, s_x, s_y, color += 2700000);
	// 	}


	big_heart(mlx_ptr, win_ptr, rx, ry, color);


	mlx_loop(mlx_ptr);
}
