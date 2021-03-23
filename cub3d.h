/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 18:39:50 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/23 19:18:00 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

#include "mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

#include "./libft/libft.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
// #define BLACK   "\033[30m"
// #define GREEN   "\033[32m"
// #define YELLOW  "\033[33m"
// #define BLUE    "\033[34m"
// #define MAGENTA "\033[35m"
// #define CYAN    "\033[36m"
// #define WHITE   "\033[37m"

#define C_TRANSP  0xFF000000
#define C_RED     0x00FF0000
#define C_GREEN   0x0000FF00
#define C_BLUE    0x000000FF
#define C_YELLOW  0x00FFFF00
#define C_CYAN    0x0000FFFF
#define C_MAGENTA 0x00FF00FF
#define C_WHITE   0x00FFFFFF
#define C_BLACK   0x00000000
#define C_T       0x01000000
#define C_R       0x00010000
#define C_G       0x00000100
#define C_B       0x00000001

#define RUN_GAME 0
#define SCREENSHOT 1

#define SPACE " \t\v\f\r"
#define FD STDOUT_FILENO
#define SPRITE '2'
#define WALL '1'
#define PLAYER "WENS"
#define INNER_OBJS "02WENS"
#define VALID_OBJS " 012WENS"

#define KEY_ESC 53
#define KEY_W 13
#define KEY_A 0
#define KEY_S 1
#define KEY_D 2
#define KEY_LEFT 123
#define KEY_RIGHT 124
#define	ROT 0.1F
#define	SPEED 0.5F
#define	FOV 1.0F

#define	NORTH 1
#define SOUTH 2
#define EAST 3
#define WEST 4

typedef struct	s_img
{
	void*	img;
	char*	addr;
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

typedef	struct	s_setting
{
	char	**words;
	char	*line;
	size_t	len;
}				t_setting;

typedef	struct	s_sprite
{
	float	x;
	float	y;
	float	dst;
}			t_sprite;

typedef	struct	s_map
{
	char		**map;
	int			fd;
	int			spr_amt;
	t_sprite	*sprites;
	size_t		height;
	size_t		width;
	t_img		cub3d;
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

typedef	struct s_wall
{
	int			type;
	int			start;
	int			end;
	int			height;
	int			put_x;
	int			put_y;
	float		ray_x;
	float		ray_y;
}	t_wall;

typedef struct	s_cub
{
	t_mlx		mlx;
	t_map		map;
	t_config	config;
	t_player	player;
	t_wall		wall;
}				t_cub;

void	exit_error(char *s);
void	get_cub_map(char *first_line, t_cub *cub);
void	render_cub(t_cub *cub);

#endif
