/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 16:53:51 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/08 20:22:41 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include "./libft/libft.h"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define SPACE	" \t\v\f\r"
#define FD			1
#define RUN_GAME	0
#define SCREENSHOT	1

enum			e_check_settings
{
	R,
	NO,
	SO,
	WE,
	EA,
	S,
	F,
	C
};

typedef struct	s_img {
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

typedef struct	s_mlx {
	void	*mlx;
	void	*win;
}				t_mlx;

typedef struct	s_setting {
	char	**words;
	size_t	len;
	char	*line;
}				t_setting;

typedef struct	s_map
{
	char	**map;
	int		fd;
}				t_map;

typedef struct s_cube {
	int		rx;
	int		ry;
	int		floor;
	int		cellar;
	t_img	north;
	t_img	south;
	t_img	west;
	t_img	east;
	t_img	sprite;
	t_mlx	mlx;
	t_map	map;
}				t_cube;

// void	set_resolution(char *r, t_mlx *vars)
// {

// 	mlx_get_screen_size(vars->mlx, &vars->rx, &vars->ry);

// }

// void	exit_perror(const char *s)
// {
// 	printf("%d\n", errno);
// 	perror(s);
// 	exit(EXIT_FAILURE);
// }

void		my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int			my_mlx_pixel_get(t_img *data, int x, int y)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	return (*(unsigned int*)dst);
}

void	exit_error(char *s)
{
	ft_putstr_fd(RED, FD);
	if (errno)
		perror(s);
	else
		ft_putendl_fd(s, FD);
	ft_putstr_fd(RESET, FD);
	exit(EXIT_FAILURE);
}

char	**free_2d_array(char **array)
{
	size_t		i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

int		check_map_type(char *path)
{
	size_t len;

	len = ft_strlen(path);
	if (len < 5)
		exit_error("Error\nWrong map file type. Expect (*.cub)");
	return (ft_strcmp(path + (len - 4), ".cub"));
}

void	check_args(int argc, char **argv, int *cub3d_mode)
{
	if (argc < 2)
		exit_error("Error\nProgram requires at least one argument (*.cub)");
	if (check_map_type(argv[1]))
		exit_error("Error\nWrong map file type. Expect (*.cub)");
	if (argc == 3)
	{
		if (!(ft_strcmp("--save", argv[2])))
		{
			*cub3d_mode = SCREENSHOT;
			return ;
		}
	}
	*cub3d_mode = RUN_GAME;
}

int		check_digits(char *s)
{
	while (*s)
	{
		if (!(ft_isdigit(*s)))
			return (0);
		s++;
	}
	return (1);
}

int		check_commas(char *s)
{
	int i;

	i = 0;
	while (*s)
	{
		if (*s == ',')
			i++;
		if (i > 2)
			return (0);
		s++;
	}
	return (i == 2 ? 1 : 0);
}

void	parse_resolution(t_setting *setting, t_cube *cube)
{
	int	x;
	int	y;

	if (cube->rx != -1 || cube->ry != -1)
		exit_error("Error\nWrong resolution format. Double definition");
	if (setting->len != 3)
		exit_error("Error\nWrong resolution format");
	if (!check_digits(setting->words[1]))
		exit_error("Error\nWrong resolution format");
	if (!check_digits(setting->words[2]))
		exit_error("Error\nWrong resolution format");
	mlx_get_screen_size(cube->mlx.mlx, &cube->rx, &cube->ry);
	x = ft_atoi(setting->words[1]);
	y = ft_atoi(setting->words[2]);
	if (x < 1 || y < 1)
		exit_error("Error\nWrong resolution range");
	if (x < cube->rx)
		cube->rx = x;
	if (y < cube->ry)
		cube->ry = y;
}

t_img	resize(t_img img, void *mlx, int new_x, int new_y)
{
	t_img	new_img;
	int		color;
	float	a;
	float	b;

	new_img.img = mlx_new_image(mlx, new_x, new_y);
	new_img.addr = mlx_get_data_addr(new_img.img, &new_img.bits_per_pixel, \
	&new_img.line_length, &new_img.endian);
	a = (float)new_x / img.width;
	b = (float)new_y / img.height;
	for (int y = 0; y < new_y; y++)
		for (int x = 0; x < new_x; x++)
		{
				color = my_mlx_pixel_get(&img, x / a, y / b);
				my_mlx_pixel_put(&new_img, x , y, color);
		}
	return (new_img);
}

void	parse_texture(void *mlx, t_setting *setting, t_img *img)
{
	if (img->img)
		exit_error("Error\nWrong texture path format. Double definition");
	if (setting->len != 2)
		exit_error("Error\nWrong texture path format");
	//printf("%s\n", setting->words[1]);
	if (!(img->img = mlx_xpm_file_to_image(mlx, setting->words[1], \
	&img->width, &img->height)))
		exit_error("Error\nFailed creating mlx image instance from file");
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, \
	&img->line_length, &img->endian);
}

int		get_color(char *color)
{
	int c;

	if (!check_digits(color))
		exit_error("Error\nWrong color format");
	c = ft_atoi(color);
	if (c < 0 || c > 255)
		exit_error("Error\nWrong color range");
	return (c);
}

void	parse_color(t_setting *setting, int *color)
{
	char	**rgb;
	int		r;
	int		g;
	int		b;

	if (*color != -1)
		exit_error("Error\nWrong color format. Double definition");
	if (!check_commas(setting->line))
		exit_error("Error\nWrong color format");
	if (!(rgb = ft_split_set(setting->line, SPACE",", &setting->len)))
		exit_error("Error\nError while creating settings array");
	if (setting->len != 4)
		exit_error("Error\nWrong color format");
	r = get_color(rgb[1]) << 16;
	g = get_color(rgb[2]) << 8;
	b = get_color(rgb[3]);
	*color = r | g | b;
	free_2d_array(rgb);
}

char	**create_map_arr(t_list **head, int size)
{
	char	**map;
	int		i;
	t_list	*tmp;

	tmp = *head;
	if (!(map = malloc((size + 1) * sizeof(char *))))
		exit_error("Error\nCan not create map array");
	map[size] = NULL;
	while (tmp)
	{
		map[--size] = tmp->content;
		tmp = tmp->next;
	}
	// i = -1;
	// while (map[++i])
	// 	printf("arr = %s\n", map[i]);
	return (map);
}

void	create_map_lst(char *first_line, t_list **map, int fd)
{
	char	*line;
	int		ret;
	t_list	*elem;

	elem = NULL;
	line = NULL;
	if (!(elem = ft_lstnew(first_line)))
		exit_error("Error\nCan not create map list");
	ft_lstadd_front(map, elem);
	while ((ret = get_next_line(fd, &line)) > 0)
	{
		if (!(elem = ft_lstnew(line)))
			exit_error("Error\nCan not create map list");
		ft_lstadd_front(map, elem);
	}
	if (ret == -1)
		exit_error("Error\nCan not read map file");
	if (!(elem = ft_lstnew(line)))
		exit_error("Error\nCan not read map file");
	ft_lstadd_front(map, elem);
}

void	get_cube_map(char *first_line, t_map *map)
{
	t_list	*map_lst;

	map_lst = NULL;
	create_map_lst(first_line, &map_lst, map->fd);
	map->map = create_map_arr(&map_lst, ft_lstsize(map_lst));
	ft_lstclear(&map_lst, free);
}

void	parse_settings(t_setting *setting, t_cube *cube)
{
	if (!(ft_strcmp("R", setting->words[0])))
		parse_resolution(setting, cube);
	else if (!(ft_strcmp("NO", setting->words[0])))
		parse_texture(cube->mlx.mlx, setting, &cube->north);
	else if (!(ft_strcmp("SO", setting->words[0])))
		parse_texture(cube->mlx.mlx, setting, &cube->south);
	else if (!(ft_strcmp("WE", setting->words[0])))
		parse_texture(cube->mlx.mlx, setting, &cube->west);
	else if (!(ft_strcmp("EA", setting->words[0])))
		parse_texture(cube->mlx.mlx, setting, &cube->east);
	else if (!(ft_strcmp("S", setting->words[0])))
		parse_texture(cube->mlx.mlx, setting, &cube->sprite);
	else if (!(ft_strcmp("C", setting->words[0])))
		parse_color(setting, &cube->cellar);
	else if (!(ft_strcmp("F", setting->words[0])))
		parse_color(setting, &cube->floor);
	else if (setting->words[0][0] == '1')
		get_cube_map(setting->line, &cube->map);

	else
		exit_error("Error\nInvalid map file configs");
}

int		get_setting(t_setting *setting, t_cube *cube)
{
	if (!(setting->words = ft_split_set(setting->line, SPACE, &setting->len)))
		exit_error("Error\nCan not create settings array");
	if (!(setting->words[0]))
	{
		free(setting->line);
		free_2d_array(setting->words);
		return (0);
	}
	parse_settings(setting, cube);
	if(!cube->map.map)
		free(setting->line);
	setting->line = NULL;
	free_2d_array(setting->words);

	return (1);
}

void	get_cub_settings(int fd, t_cube *cube)
{
	int			ret;
	t_setting	setting;

	setting.line = NULL;
	while ((ret = get_next_line(fd, &setting.line)) > 0)
		get_setting(&setting, cube);
	if (ret == -1)
		exit_error("Error\nCan not read map file");
	get_setting(&setting, cube);
}



void	init_cube(t_cube *cube)
{
	if (!(cube->mlx.mlx = mlx_init()))
		exit_error("Error\nMlx init error");
	errno = 0;
	cube->rx = -1;
	cube->ry = -1;
	cube->floor = -1;
	cube->cellar = -1;
	cube->north.img = NULL;
	cube->south.img = NULL;
	cube->east.img = NULL;
	cube->west.img = NULL;
	cube->sprite.img = NULL;
	cube->map.map = NULL;
}

void	check_cube_settings(t_cube *cube)
{
	if (cube->rx == -1|| cube->ry == -1)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing resolution");
	if (cube->cellar == -1 || cube->floor == -1)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing floor or celling color");
	if (!cube->north.img || !cube->south.img)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing wall texture path");
	if (!cube->east.img || !cube->west.img)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing wall texture path");
	if (!cube->sprite.img)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing sprite texture path");
}
int		main(int argc, char **argv)
{
	int		cub3d_mode;
	t_cube	cube;

	check_args(argc, argv, &cub3d_mode);
	init_cube(&cube);

	if ((cube.map.fd = open(argv[1], O_RDONLY)) == -1)
		exit_error("Error\nOpen map file failed");


	get_cub_settings(cube.map.fd, &cube);
	check_cube_settings(&cube);

	//printf("%d %d\n", cube.rx, cube.ry);
	//printf("mlx %s\n", cube.mlx.mlx);
	// printf("\ncellar - %06x\n", cube.cellar);
	// printf("\nfloor  - %06x\n", cube.floor);
	// printf("\nrx = %d ry = %d\n", cube.rx, cube.ry);
	//create_map_lst(argv[1], &map, fd);

	//create_map_arr(&map, ft_lstsize(map));

	sleep(30);
}
