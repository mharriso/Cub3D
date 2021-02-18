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

int		create_trgb(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

int		get_t(int trgb)
{
	return (trgb & (0xFF << 24));
}

int		get_r(int trgb)
{
	return (trgb & (0xFF << 16));
}

int		get_g(int trgb)
{
	return (trgb & (0xFF << 8));
}

int		get_b(int trgb)
{
	return (trgb & 0xFF );
}

int main()
{
	int		x;
	int		y;
	int		color;
	int		save;
	int		buf;
	srand(time(NULL));
	int trgb = create_trgb(BLUE, BLUE, BLUE, BLUE);
	// printf("%08x\n", trgb);
	// printf("%08x\n", get_t(trgb));
	// printf("%08x\n", get_r(trgb));
	// printf("%08x\n", get_g(trgb));
	// printf("%08x\n", get_b(trgb));
	// printf("%08x\n", WHITE & WHITE);
	// printf("%08x\n", BLUE & RED);
	// printf("%08x\n", BLUE & GREEN);
	// printf("%08x\n", BLUE & BLUE);
	// printf("%08x\n", BLUE & WHITE);

	// printf("%08x\n", BLUE & 0x00010101);
	// printf("%08x\n", RED ^ 0x00010101);
	// printf("%08x\n", RED & 0x00010101);
	// printf("%08x\n", RED | 0x00010101);
	// printf("%08x\n", 0x00010101 - RED & 0x00010101);
	//printf("%08x\n", CYAN | 0x00010101);
	// color = B;
	// printf("%08x\n", color);
	// buf = color;
	// color <<= 1;
	// printf("%08x\n", color);
	// color |= buf;
	// printf("%08x\n", color);

	color = RED;
	for (int i = 0; i < 50; i++)
		 printf("%08x\n", color--);


	// for(int n = 0; n < 24; n+=8)
	// 	printf("%08x\n", 1 << n);
	// for(int i = 0; i < 20;i++)
	// {
	// 	int n = 8 * (rand()%3);

	// 	printf("n = %d\n", n);
	// 	printf("%08x\n", 1 << n);
	// }

}
