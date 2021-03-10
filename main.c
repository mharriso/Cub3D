/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 16:53:51 by mharriso          #+#    #+#             */
/*   Updated: 2021/03/10 19:18:19 by mharriso         ###   ########.fr       */
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

#define SPACE " \t\v\f\r"
#define FD 1
#define RUN_GAME 0
#define SCREENSHOT 1
#define SPRITE '2'
#define WALL '1'
#define PLAYER "WENS"
#define INNER_OBJS "02WENS"
#define VALID_OBJS " 012WENS"

enum		e_check_settings
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
	char	*line;
	size_t	len;
}				t_setting;

typedef struct	s_map
{
	char	**map;
	int		fd;
	int		spr_amt;
	size_t	height;
	size_t	width;
}				t_map;

typedef struct	s_player
{
	float x;
	float y;
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


typedef struct	s_cube {

	t_mlx		mlx;
	t_map		map;
	t_config	config;
	t_player	player;
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
void	check_cub_settings(t_cube *cub)
{
	if (cub->config.rx == -1 || cub->config.ry == -1)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing resolution");
	if (cub->config.ceiling == -1)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing ceiling color");
	if (cub->config.floor == -1)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing floor color");
	if (!cub->config.north.img || !cub->config.south.img \
		|| !cub->config.east.img || !cub->config.west.img)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing wall texture path");
	if (!cub->config.sprite.img)
		exit_error("Error\nToo few arguments in map file. \
		\nMissing sprite texture path");
}
void	parse_resolution(t_setting *setting, t_cube *cub)
{
	int	x;
	int	y;

	if (cub->config.rx != -1 || cub->config.ry != -1)
		exit_error("Error\nWrong resolution format. Double definition");
	if (setting->len != 3)
		exit_error("Error\nWrong resolution format");
	if (!check_digits(setting->words[1]))
		exit_error("Error\nWrong resolution format");
	if (!check_digits(setting->words[2]))
		exit_error("Error\nWrong resolution format");
	mlx_get_screen_size(cub->mlx.mlx, &cub->config.rx, &cub->config.ry);
	x = ft_atoi(setting->words[1]);
	y = ft_atoi(setting->words[2]);
	if (x < 1 || y < 1)
		exit_error("Error\nWrong resolution range");
	if (x < cub->config.rx)
		cub->config.rx = x;
	if (y < cub->config.ry)
		cub->config.ry = y;
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
	// 	printf("%2d) %s\n", i, map[i]);
	return (map);
}

void	create_map_lst(char *first_line, t_list **map_lst, t_map *map)
{
	char	*line;
	int		ret;
	t_list	*elem;

	elem = NULL;
	line = NULL;
	if (!(elem = ft_lstnew(first_line)))
		exit_error("Error\nCan not create map list");
	ft_lstadd_front(map_lst, elem);
	while ((ret = get_next_line(map->fd, &line)) > 0 && line[0])
	{
		if (!(elem = ft_lstnew(line)))
			exit_error("Error\nCan not create map list");
		ft_lstadd_front(map_lst, elem);
		map->height++;
	}
	if (ret == -1)
		exit_error("Error\nCan not read map file");
	if (ret != 0)
		exit_error("Error\nInvalid map. Extra lines.");
	if (!(elem = ft_lstnew(line)))
		exit_error("Error\nCan not read map file");
	ft_lstadd_front(map_lst, elem);
}

void	check_border(t_map *map, int x, int y)
{
	if (x == map->width - 1 || y == map->height - 2)
		exit_error("Error\nInvalid map. Gap detected!");
	if (x == 0 || y == 0)
		exit_error("Error\nInvalid map. Gap detected!");
	if (x > 0 && (map->map)[y][x - 1] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (x < map->width - 1 && (map->map)[y][x + 1] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (y > 0 && (map->map)[y - 1][x] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
	if (y < map->height - 1 && (map->map)[y + 1][x] == ' ')
		exit_error("Error\nInvalid map. Gap detected!");
}
void	check_gaps(t_map map, int x, int y)
{

}
void	set_player(t_map *map, char dir, int x, int y)
{
	if (dir == 'N')
	{

	}
	else if (dir == 'S')
	{

	}
	else if (dir == 'E')
	{

	}
	else if (dir == 'W')
	{

	}
}

void	parse_map(t_map *map)
{
	int	x;
	int	y;

	y = 0;
	printf("height = %zu\n", map->height);
	while (y < map->height)
	{
		x = 0;
		map->width = ft_strlen(map->map[y]);
		printf("%d) %s\n", y, map->map[y]);
		while (x < map->width)
		{
			if (!ft_strchr(VALID_OBJS, map->map[y][x]))
				exit_error("Error\nInvalid map. Invalid object detected!");
			if (ft_strchr(INNER_OBJS, (map->map)[y][x]))
				check_border(map, x, y);
			if (map->map[y][x] == SPRITE)
				map->spr_amt++;
			if (ft_strchr(PLAYER, map->map[y][x]))
				set_player(map, map->map[y][x], x, y);
			x++;
		}
		y++;
	}
	printf("sprites = %d\n", map->spr_amt);
}

void	get_cub_map(char *first_line, t_map *map)
{
	t_list	*map_lst;

	map_lst = NULL;
	create_map_lst(first_line, &map_lst, map);
	map->height = ft_lstsize(map_lst);
	map->map = create_map_arr(&map_lst, map->height);
	parse_map(map);
	ft_lstclear(&map_lst, NULL);
}

void	parse_settings(t_setting *setting, t_cube *cub)
{
	if (!(ft_strcmp("R", setting->words[0])))
		parse_resolution(setting, cub);
	else if (!(ft_strcmp("NO", setting->words[0])))
		parse_texture(cub->mlx.mlx, setting, &cub->config.north);
	else if (!(ft_strcmp("SO", setting->words[0])))
		parse_texture(cub->mlx.mlx, setting, &cub->config.south);
	else if (!(ft_strcmp("WE", setting->words[0])))
		parse_texture(cub->mlx.mlx, setting, &cub->config.west);
	else if (!(ft_strcmp("EA", setting->words[0])))
		parse_texture(cub->mlx.mlx, setting, &cub->config.east);
	else if (!(ft_strcmp("S", setting->words[0])))
		parse_texture(cub->mlx.mlx, setting, &cub->config.sprite);
	else if (!(ft_strcmp("C", setting->words[0])))
		parse_color(setting, &cub->config.ceiling);
	else if (!(ft_strcmp("F", setting->words[0])))
		parse_color(setting, &cub->config.floor);
	else if (setting->words[0][0] == '1')
	{
		check_cub_settings(cub);
		get_cub_map(setting->line, &cub->map);
	}
	else
		exit_error("Error\nInvalid map file configs");
}

int		get_setting(t_setting *setting, t_cube *cub)
{
	if (!(setting->words = ft_split_set(setting->line, SPACE, &setting->len)))
		exit_error("Error\nCan not create settings array");
	if (!(setting->words[0]))
	{
		free(setting->line);
		free_2d_array(setting->words);
		return (0);
	}
	parse_settings(setting, cub);
	if (!cub->map.map)
		free(setting->line);
	setting->line = NULL;
	free_2d_array(setting->words);

	return (1);
}

void	get_cub_settings(char *path, t_cube *cub)
{
	int			ret;
	t_setting	setting;

	if ((cub->map.fd = open(path, O_RDONLY)) == -1)
		exit_error("Error\nOpen map file failed");
	setting.line = NULL;
	while ((ret = get_next_line(cub->map.fd, &setting.line)) > 0)
		get_setting(&setting, cub);
	if (ret == -1)
		exit_error("Error\nCan not read map file");
	get_setting(&setting, cub);
}



void	init_cub(t_cube *cub)
{
	if (!(cub->mlx.mlx = mlx_init()))
		exit_error("Error\nMlx init error");
	errno = 0;
	cub->config.rx = -1;
	cub->config.ry = -1;
	cub->config.floor = -1;
	cub->config.ceiling = -1;
	cub->config.north.img = NULL;
	cub->config.south.img = NULL;
	cub->config.east.img = NULL;
	cub->config.west.img = NULL;
	cub->config.sprite.img = NULL;
	cub->map.map = NULL;
	cub->map.height = 0;
	cub->map.spr_amt = 0;
}


int		main(int argc, char **argv)
{
	int		cub3d_mode;
	t_cube	cub;

	check_args(argc, argv, &cub3d_mode);
	init_cub(&cub);
	get_cub_settings(argv[1], &cub);

	//printf("%d %d\n", cub.rx, cub.ry);
	//printf("mlx %s\n", cub.mlx.mlx);
	// printf("\nceiling - %06x\n", cub.ceiling);
	// printf("\nfloor  - %06x\n", cub.floor);
	// printf("\nrx = %d ry = %d\n", cub.rx, cub.ry);
	//create_map_lst(argv[1], &map, fd);

	//create_map_arr(&map, ft_lstsize(map));

	//sleep(30);
}
