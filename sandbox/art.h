#ifndef ART_H
# define ART_H

#include <stdlib.h>
#include <time.h>
#include <mlx.h>
#include <stdio.h>

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
    void*	img;
    char*	addr;
    int		bits_per_pixel;
    int		line_length;
    int		endian;
	int		height;
	int		width;
	int		x;
	int		y;
}               t_img;

typedef struct  s_vars {
    void	*mlx;
    void	*win;
	int		rx;
	int		ry;
}				t_vars;

typedef struct	s_art
{
	t_vars	*vars;
	t_img	*back;
	t_img	*obj;
}				t_art;

void	cmyk(t_img img, int rx, int ry);
void	mix_stripes(t_img img, int rx, int ry, int size);
void	mix_cylinders(t_img img, int rx, int ry, int size);
void	random_gradient(t_img img, int rx, int ry);
void	mix(t_img img, int rx, int ry, int size);
void	gradient(t_img img, int rx, int ry);

int		invert_color(int color);
void	my_mlx_pixel_put(t_img *data, int x, int y, int color);
int		my_mlx_pixel_get(t_img *data, int x, int y);
#endif
