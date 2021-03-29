/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 18:39:50 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/29 19:50:35 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "mlx.h"
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <math.h>

# include "./libft/libft.h"

# define C_TRANSP  0xFF000000
# define C_RED     0x00FF0000
# define C_GREEN   0x0000FF00
# define C_BLUE    0x000000FF
# define C_YELLOW  0x00FFFF00
# define C_CYAN    0x0000FFFF
# define C_MAGENTA 0x00FF00FF
# define C_WHITE   0x00FFFFFF
# define C_BLACK   0x00000000
# define C_T       0x01000000
# define C_R       0x00010000
# define C_G       0x00000100
# define C_B       0x00000001

# define RUN_GAME 0
# define SCREENSHOT 1

# define SPACE " \t\v\f\r"
# define SPRITE '2'
# define WALL '1'
# define PLAYER "WENS"
# define INNER_OBJS "02WENS"
# define VALID_OBJS " 012WENS"

# define ROT 0.1F
# define SPEED 0.5F
# define FOV 1.1F
# define SHADOW 10

# define NORTH 1
# define SOUTH 2
# define EAST 3
# define WEST 4

typedef struct	s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		height;
	int		width;
	int		x;
	int		y;
}				t_img;

typedef struct	s_mlx
{
	void	*mlx;
	void	*win;
}				t_mlx;

typedef struct	s_setting
{
	char	**words;
	char	*line;
	size_t	len;
}				t_setting;

typedef struct	s_sprite
{
	float	x;
	float	y;
	float	dst;
}				t_sprite;

typedef struct	s_sprite_rend
{
	int		color;
	int		x;
	int		y;
	int		x_start;
	int		y_start;
	float	a;
	float	b;
}				t_sprite_rend;

typedef struct	s_map
{
	char		**map;
	int			fd;
	int			spr_amt;
	t_sprite	*sprites;
	size_t		height;
	size_t		width;
	t_img		cub3d;
	int			spr_size;
	float		spr_dir;
	float		spr_dst;
}				t_map;

typedef struct	s_player
{
	float pos_x;
	float pos_y;
	float angle;
}				t_player;

typedef struct	s_config
{
	int		rx;
	int		ry;
	int		floor;
	int		ceiling;
	t_img	north;
	t_img	south;
	t_img	west;
	t_img	east;
	t_img	sprite;
}				t_config;

typedef struct	s_wall
{
	int			type;
	int			start;
	int			end;
	int			height;
	int			put_x;
	int			put_y;
	float		*rays;
	float		dst;
	float		k_x;
	float		k_y;
	float		ray_x;
	float		ray_y;
}				t_wall;

typedef struct	s_bmp
{
	int			fd;
	int			data_size;
	int			padding_size;
}				t_bmp;

typedef struct	s_cub
{
	t_mlx		mlx;
	t_map		map;
	t_config	config;
	t_player	player;
	t_wall		wall;
}				t_cub;

void			get_cub_settings(char *path, t_cub *cub);
void			parse_color(t_setting *setting, int *color);
void			parse_resolution(t_setting *setting, t_cub *cub);
void			get_cub_map(char *first_line, t_cub *cub);
void			parse_map_1(t_cub *cub);
void			parse_map_2(t_map *map);
int				key_handler(int key, t_cub *cub);
void			render_cub(t_cub *cub);
void			ray_loop(t_cub *cub);
void			render_wall(t_cub *cub, float a);
void			render_sprites(t_cub *cub);
int				my_mlx_pixel_get(t_img *data, int x, int y);
void			my_mlx_pixel_put(t_img *data, int x, int y, int color);
void			shadow_effect(int k, int *color);
void			check_angle(float *a);
void			sort_array(t_sprite *arr, int n);
char			**free_2d_array(char **array);
void			take_screenshot(t_cub *cub);
void			create_bmp_file(t_img *cub3d, int x, int y);
int				close_win(t_mlx *mlx);
void			exit_error(char *s);

#endif
